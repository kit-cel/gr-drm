# -*- coding: utf-8 -*-
"""
Created on Thu Jan 14 10:49:57 2016

@author: Julian Wetzel
"""

from PyQt4 import QtGui
import sys
import drm_options
import subprocess
import transmission_options
import drm_transmitter



class DRMMainWindow(QtGui.QMainWindow,drm_options.Ui_MainWindow):

    def __init__(self,parent=None):
        super(DRMMainWindow,self).__init__(parent)
        self.setupUi(self)
        self.search_file_btn.clicked.connect(self.browse_file)
        self.actionOpen.triggered.connect(self.popup)
        self.actionTransmission_Options.triggered.connect(self.popup)
        self.start_btn.clicked.connect(self.getParameters)
        self.stop_btn.clicked.connect(self.stopTransmission)
        self.pulse_check.stateChanged.connect(self.disableSearchFileBtn)
        self.actionAbout.triggered.connect(self.showAbout)
        self.rm_box.addItems(["A","B","C","D"])
        self.so_box.addItems(["0","1","2","3","4","5"])
        self.audio_samp.addItems(["12 KHz","24 KHz", "48 KHz"])
        self.rm_box.activated.connect(self.rm_so_config)
        self.transmissionOptionsBtn.clicked.connect(self.popup)
        self.setParameters()


    def stopTransmission(self):
        self.tb.stop()
        self.tb.wait()

    def disableSearchFileBtn(self):
        self.search_file_btn.setDisabled(self.pulse_check.isChecked())

    def browse_file(self):
        self.file_path.clear()
        directory = QtGui.QFileDialog.getOpenFileName(self, 'Open file','/home')
        self.file_path.insert(directory)

    def popup(self):
        self.trans_dialog = DRMTransOptionsDialog()
        if self.trans_dialog.exec_():
            DRMParameters.output_name = str(self.trans_dialog.output_name.text())
            DRMParameters.center_freq = self.trans_dialog.center_freq.value()
            DRMParameters.samp_rate_usrp = self.trans_dialog.samp_rate_usrp.value()
            DRMParameters.gen_output = self.trans_dialog.gen_file.isChecked()
            if  DRMParameters.uhd_found == True and self.trans_dialog.usrp_select.currentIndex() < len(usrp.name):
                DRMParameters.usrp_name = str(usrp.name[self.trans_dialog.usrp_select.currentIndex()])
                DRMParameters.usrp_id = str(usrp.identification[self.trans_dialog.usrp_select.currentIndex()])
            else:
                DRMParameters.uhd_found = False

    def getParameters(self):
        DRMParameters.audio_file = str(self.file_path.text())
        DRMParameters.text_msg = str(self.text_message.text())
        DRMParameters.station_label = str(self.station_label.text())
        DRMParameters.audio_samp = mode_dicts.audio_rates[str(self.audio_samp.currentText())]
        DRMParameters.pulse_audio = self.pulse_check.isChecked()
        DRMParameters.so = int(self.so_box.currentText())
        DRMParameters.rm = mode_dicts.rm_modes[str(self.rm_box.currentText())]
        if self.msc_16.isChecked():
            DRMParameters.msc_mod = 1
        else:
            DRMParameters.msc_mod = 2
        if self.sdc_4.isChecked():
            DRMParameters.sdc_mod = 1
        else:
            DRMParameters.sdc_mod = 0
        # init drm_transmitter
        self.start_transmission()

    def setParameters(self):
        self.file_path.setText(DRMParameters.audio_file)
        self.text_message.setText(DRMParameters.text_msg)
        self.station_label.setText(DRMParameters.station_label)
        self.audio_samp.setCurrentIndex(1)
        self.so_box.setCurrentIndex(DRMParameters.so)
        self.rm_box.setCurrentIndex(DRMParameters.rm)
        self.pulse_check.setChecked(DRMParameters.pulse_audio)
        if DRMParameters.msc_mod == 1:
            self.msc_16.setChecked(True)
        else:
            self.msc_64.setChecked(True)
        if DRMParameters.sdc_mod == 1:
            self.sdc_4.setChecked(True)
        else:
            self.sdc_16.setChecked(True)

    def start_transmission(self):
        try:
            self.tb = drm_transmitter.drm_transmitter(DRMParameters)
            self.tb.start()

        except RuntimeError as config_error:
            self.start_btn.show()
            self.stop_btn.hide()
            QtGui.QMessageBox.warning(self,"Error in Configuration",str(config_error),QtGui.QMessageBox.Ok)

    def rm_so_config(self):
        self.so_box.clear()
        DRMParameters.rm = self.rm_box.currentText()
        if DRMParameters.rm == "A" or DRMParameters.rm == "B":
            self.so_entries = ["1","2","3","4","5"]
        else:
            self.so_entries = ["3","5"]
        self.so_box.addItems(self.so_entries)

    def showAbout(self):
        about = QtGui.QMessageBox()
        about.setIcon(QtGui.QMessageBox.Information)
        about.setText("This is a graphical user interface written in python for the DRM transmitter implemented in GNURadio")
        about.setInformativeText("Copyright (C) 2016 Karlsruhe Institute of Technology CEL Julian Wetzel")
        about.setWindowTitle("About GNURadio DRM Transmitter")
        about.setStandardButtons(QtGui.QMessageBox.Ok)
        about.exec_()


class DRMTransOptionsDialog(QtGui.QDialog,transmission_options.Ui_Dialog):
    def __init__(self):
        super(DRMTransOptionsDialog, self).__init__(parent=None)
        self.setupUi(self)
        self.output_name.setText(DRMParameters.output_name)
        self.center_freq.setValue(DRMParameters.center_freq)
        self.samp_rate_usrp.setValue(DRMParameters.samp_rate_usrp)
        self.gen_file.setChecked(DRMParameters.gen_output)
        if not DRMParameters.gen_output:
            self.output_name.hide()
            self.label_5.hide()
        self.gen_file.stateChanged.connect(self.hide_out_path)
        self.usrp_select.clear()
        del usrp.identification[:]
        del usrp.networked[:]
        del usrp.name[:]
        # Look for usrps connected to host
        try:
            output = subprocess.check_output('uhd_find_devices').splitlines()
            self.usrp_select.setEnabled(True)
            DRMParameters.uhd_found = True
            self.center_freq.setEnabled(True)
            self.samp_rate_usrp.setEnabled(True)
            for line in output:
                line = line.decode('UTF-8')
                if not line.find('name')==-1:
                    usrp.name.append('name='+line[line.find('name')+6:])
                elif not line.find('addr')==-1:
                    usrp.identification.append('addr='+line[line.find('addr')+6:])
                    usrp.networked.append(True)
                elif not line.find('serial')==-1:
                    usrp.identification.append('serial='+line[line.find('serial')+8:])
                    usrp.networked.append(False)
            print "USRP name:", usrp.name, "identification:", usrp.identification, "networked series:", usrp.networked
            for x in range(0, len(usrp.name)):
                self.usrp_select.addItem(usrp.identification[x]+' ('+usrp.name[x]+')')
            self.usrp_select.addItem('No USRP')

        except:
            self.usrp_select.setEnabled(False)
            DRMParameters.uhd_found = False
            self.usrp_select.addItem('No USRP found')
            self.center_freq.setEnabled(False)
            self.samp_rate_usrp.setEnabled(False)

    def hide_out_path(self):
        if self.gen_file.isChecked():
           self.output_name.show()
           self.label_5.show()
        else:
            self.output_name.hide()
            self.label_5.hide()

class DRMParameters:
    # default values for transmission
    output_name = 'drm_out.wav'
    center_freq = 5
    samp_rate_usrp = 250
    msc_mod = 1
    sdc_mod = 1
    rm = 1
    so = 3
    text_msg = 'Hello! Hallo!'
    station_label = 'DRM on GNURadio'
    audio_samp = 24
    audio_file = ''
    gen_output = False
    uhd_found = False
    usrp_id = ''
    usrp_name = ''
    pulse_audio = False

class usrp:
    name=[]
    identification=[]
    networked=[]
    
class mode_dicts:
    rm_modes = {"A" : 0, "B" : 1, "C" : 2, "D" : 3}
    audio_rates = {"12 KHz" : 12, "24 KHz" : 24,"48 KHz" : 48}


def main():
    app = QtGui.QApplication(sys.argv)
    form = DRMMainWindow()
    form.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()

# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'transmission_options.ui'
#
# Created: Tue Jan 19 10:58:22 2016
#      by: PyQt4 UI code generator 4.10.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName(_fromUtf8("Dialog"))
        Dialog.resize(450, 290)
        self.verticalLayout = QtGui.QVBoxLayout(Dialog)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.label = QtGui.QLabel(Dialog)
        font = QtGui.QFont()
        font.setPointSize(14)
        font.setBold(True)
        font.setWeight(75)
        self.label.setFont(font)
        self.label.setObjectName(_fromUtf8("label"))
        self.verticalLayout.addWidget(self.label)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.label_2 = QtGui.QLabel(Dialog)
        font = QtGui.QFont()
        font.setPointSize(10)
        self.label_2.setFont(font)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.horizontalLayout.addWidget(self.label_2)
        self.center_freq = QtGui.QDoubleSpinBox(Dialog)
        self.center_freq.setDecimals(3)
        self.center_freq.setProperty("value", 5.0)
        self.center_freq.setObjectName(_fromUtf8("center_freq"))
        self.horizontalLayout.addWidget(self.center_freq)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName(_fromUtf8("horizontalLayout_2"))
        self.label_3 = QtGui.QLabel(Dialog)
        font = QtGui.QFont()
        font.setPointSize(10)
        self.label_3.setFont(font)
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.horizontalLayout_2.addWidget(self.label_3)
        self.samp_rate_usrp = QtGui.QDoubleSpinBox(Dialog)
        self.samp_rate_usrp.setDecimals(3)
        self.samp_rate_usrp.setMaximum(1000.0)
        self.samp_rate_usrp.setProperty("value", 250.0)
        self.samp_rate_usrp.setObjectName(_fromUtf8("samp_rate_usrp"))
        self.horizontalLayout_2.addWidget(self.samp_rate_usrp)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        self.usrp_select = QtGui.QComboBox(Dialog)
        self.usrp_select.setObjectName(_fromUtf8("usrp_select"))
        self.verticalLayout.addWidget(self.usrp_select)
        self.label_4 = QtGui.QLabel(Dialog)
        font = QtGui.QFont()
        font.setPointSize(14)
        font.setBold(True)
        font.setWeight(75)
        self.label_4.setFont(font)
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.verticalLayout.addWidget(self.label_4)
        self.gen_file = QtGui.QCheckBox(Dialog)
        self.gen_file.setChecked(True)
        self.gen_file.setObjectName(_fromUtf8("gen_file"))
        self.verticalLayout.addWidget(self.gen_file)
        self.label_5 = QtGui.QLabel(Dialog)
        self.label_5.setObjectName(_fromUtf8("label_5"))
        self.verticalLayout.addWidget(self.label_5)
        self.output_name = QtGui.QLineEdit(Dialog)
        self.output_name.setObjectName(_fromUtf8("output_name"))
        self.verticalLayout.addWidget(self.output_name)
        self.buttonBox = QtGui.QDialogButtonBox(Dialog)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName(_fromUtf8("buttonBox"))
        self.verticalLayout.addWidget(self.buttonBox)

        self.retranslateUi(Dialog)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL(_fromUtf8("rejected()")), Dialog.reject)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL(_fromUtf8("accepted()")), Dialog.accept)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        Dialog.setWindowTitle(_translate("Dialog", "Transmission Options", None))
        self.label.setText(_translate("Dialog", "USRP", None))
        self.label_2.setText(_translate("Dialog", "Center Frequency [MHz]", None))
        self.label_3.setText(_translate("Dialog", "Sample rate [kHz]", None))
        self.label_4.setText(_translate("Dialog", "Output File", None))
        self.gen_file.setText(_translate("Dialog", "Generate Output File", None))
        self.label_5.setText(_translate("Dialog", "File Name", None))



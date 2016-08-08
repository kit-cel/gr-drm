function [data_depart] = drm_mlc_departitioning(data_partitioned, MSC)
% Departitions input data. NOTE: In this special case, only MSC has to be
% departitioned!

data_depart = cell(3,1);

% concatenate the decoded streams (higher protected part (coming from C_0) first)
for i = 1 : MSC.M_TF
    data_depart{i} = [ data_partitioned{i}{1} data_partitioned{i}{2} ];
end

end



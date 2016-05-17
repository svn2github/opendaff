%
%  OpenDAFF
%

function [] = daff_write_metadata( fid, metadata )
%DAFF_WRITE_METADATA Write a DAFF metadata block into DAFF binary file
    
    % Number of keys
    fwrite(fid, length(metadata), 'int32');
    
    % Write the keys
    for i=1:length(metadata)
        fwrite(fid, metadata(i).datatype, 'int32');
        fwrite(fid, metadata(i).name, 'char');
        fwrite(fid, 0, 'char');
        switch metadata(i).datatype
            case 0 % DAFF_BOOL
                if (metadata(i).value == true)
                    fwrite(fid, 1, 'int32');
                else
                    fwrite(fid, 0, 'int32');
                end

            case 1 % DAFF_INT
                fwrite(fid, metadata(i).value, 'int32');
                
            case 2 % DAFF_FLOAT
                fwrite(fid, metadata(i).value, 'double');
                
            case 3 % DAFF_STRING
                fwrite(fid, metadata(i).value, 'char');
                fwrite(fid, 0, 'char');
        end
    end
end

%
%  OpenDAFF
%
%  File:    daff_write.m
%  Purpose: Writer implementation for DAFF files
%  Author:  Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
%
%  $Id: daff_write.m,v 1.7 2010/03/08 14:32:41 stienen Exp $
%

function [] = daff_write( varargin )
%DAFF_WRITE Create DAFF files
%   ...
%
%  --= General parameters =--
%
%  -- Required --
%
%  filename     char        Output filename (*.daff)
%  content      char        Content type
%                           ('IR' => Impulse responses,
%                            'MS' => Magnitude spectra)
%  datafunc     function    Data function (delivers the data for a direction)
%  orient       vector-3    Orientation [yaw pitch roll] angles [°]                     
%
%  -- Optional --
%
%  basepath     char        Base path for all input files (default: none)
%  metadata     struct      Metadata information (see daff_metadata_addKey)
%    
%   
%   mdist           Measurement distance [m]
%   reference       Reference value [dB]
%
%   
%  
%
%   
%
%   --= Impulse response content parameters =--
%
%   samplerate      Sampling rate [Hertz]
%   quantization    Element quantization (int16|int24|float32)
%   zthreshold      Detection threshold for zero-coefficients (default: -inf)
%
%   Options for magnitude spectra
%

    % --= Option definitions =--
    
    % Options with logical arguments (true|false)
    boolarg = {};
    
    % Options with integer number > 0 arguments
    ingzarg = {'alphapoints', 'betapoints', 'channels'};
    
   % Options with floating point number arguments
    floatarg = {'reference', 'zthreshold'};
             
    % Options with floating point number >= 0 arguments
    pfloatarg = {'alphares', 'betares', 'mdist', 'samplerate'};

    floatvecarg = {'alpharange', 'betarange', 'orient'};
             
    % Options with string parameters
    strarg = {'filename', 'basepath', 'content', 'quantization'};
      
    % Options without an argument
    nonarg = {'dummy'};
    
    % Options with one argument
    onearg = [boolarg ingzarg floatarg pfloatarg floatvecarg strarg 'datafunc' 'metadata'];
    
    % Required options
    reqarg = {'filename', 'content', 'datafunc', 'channels', 'orient'};    
    
    % +------------------------------------------------+
    % |                                                |
    % |   Parsing and validation of input parameters   |
    % |                                                |
    % +------------------------------------------------+
    
    % Parse the arguments
    args = struct();
    for i=1:length(nonarg), args.(nonarg{i}) = false; end
    
    i=1;
    while i<=nargin
        if ~ischar(varargin{i}), error(['Parameter ' num2str(i) ': String expected']); end
        key = lower(varargin{i});
        i = i+1;
        r = nargin-i+1; % Number of remaining arguments
        
        switch key
        case nonarg
            args.(key) = true;
        
        % Options with one argument
        case onearg
            if (r < 1), error(['Option ''' key ''' requires an argument']); end
            args.(key) = varargin{i};
            i = i+1;
            
        otherwise
            error(['Invalid option (''' key ''')']);
        end        
    end
    
    % Validate the arguments
    for i=1:length(reqarg)
        key = reqarg{i};
        if ~isfield(args, key), error(['Option ''' key ''' must be specified']); end
    end
    
    for i=1:length(boolarg)
        key = boolarg{i};
        if isfield(args, key)
            if ~islogical(args.(key))
                error(['Argument for option ''' key ''' must be logical']);
            else
                % Type cast
                args.(key) = logical( args.(key) );
            end
        end
    end
    
    for i=1:length(ingzarg)
        key = ingzarg{i};
        if isfield(args, key)
            if (~isscalar(args.(key)) || ~any(isfinite(args.(key))) || ~isreal(args.(key)) || (ceil(args.(key)) ~= args.(key)) || (args.(key) <= 0))
                error(['Argument for option ''' key ''' must be an integer > 0']);
            else
                % Type cast
                args.(key) = int32( args.(key) );
            end
        end
    end
 
    for i=1:length(floatarg)
        key = floatarg{i};
        if isfield(args, key)
            if (~isscalar(args.(key)) || ~any(isfinite(args.(key))) || ~isreal(args.(key)))
                error(['Argument for option ''' key ''' must be a real number']);
            else
                % Type cast
                args.(key) = double( args.(key) );
            end
        end
    end
   
    for i=1:length(pfloatarg)
        key = pfloatarg{i};
        if isfield(args, key)
            if (~isscalar(args.(key)) || ~any(isfinite(args.(key))) || ~isreal(args.(key)) || (args.(key) < 0))
                error(['Argument for option ''' key ''' must be a non-negative real number']);
            else
                % Type cast
                args.(key) = double( args.(key) );
            end
        end
    end
    
    for i=1:length(floatvecarg)
        key = floatvecarg{i};
        if isfield(args, key)
            if (~isvector(args.(key)) || ~any(isfinite(args.(key))) || ~isreal(args.(key)))
                error(['Argument for option ''' key ''' must be a vector of real numbers']);
            else
                % Type cast
                args.(key) = double( args.(key) );
            end
        end
    end
    
    for i=1:length(strarg)
        key = strarg{i};
        if isfield(args, key)
            if (~ischar(args.(key)) || (length(args.(key)) == 0))
                error(['Argument for option ''' key ''' must be a non-empty string']);
            end
        end
    end
    
    % More validation ;-)
    
    % Content
    args.content = upper(args.content);
    switch args.content
        case 'IR'
            contentStr = 'Impulse responses';
            contentType = 0; % DAFF_IMPULSE_RESPONSE
        case 'MS'
            contentStr = 'Magnitude spectra';
            contentType = 1; % DAFF_MAGNITUDE_SPECTRUM
        
        otherwise
            error(['Invalid content type (' args.content ')']);
    end
    
    % Metadata
    if ~isfield(args, 'metadata'), args.metadata = []; end;
    
    % Angular ranges default values
    if ~isfield(args, 'alpharange'), args.alpharange = [0 360]; end
    if ~isfield(args, 'betarange'), args.betarange = [0 180]; end
        
    % Correct angular range ordering
    alphastart = min(args.alpharange);
    alphaend = max(args.alpharange);
    betastart = min(args.betarange);
    betaend = max(args.betarange);
    
    if ((alphastart < 0) || (alphastart > 360))
        error('Alpha range values must lie within the interval [0, 360]');
    end
    
    if ((betastart < 0) || (betastart > 180))
        error('Beta range values must lie within the interval [0, 180]');
    end  
    alphaspan = alphaend - alphastart;
    betaspan = betaend - betastart;
    
    % Alpha points and resolution
    if (~isfield(args, 'alphapoints') && ~isfield(args, 'alphares'))
        error('You must specify ''alphapoints'' or ''alphares''');
    end
    
    if (isfield(args, 'alphapoints') && isfield(args, 'alphares'))
        error('Specify either ''alphapoints'' or ''alphares'', but not both');
    end
    
    if isfield(args, 'alphares')
        args.alphapoints = alphaspan / args.alphares;
        if (ceil(args.alphapoints) ~= args.alphapoints)
            error('Alpha range and alpha resolution are not an integer multiple')
        end
    else
        args.alphares = alphaspan / args.alphapoints;
    end
   
    % Beta points and resolution
    if (~isfield(args, 'betapoints') && ~isfield(args, 'betares'))
        error('You must specify ''betapoints'' or ''betares''');
    end
    
    if (isfield(args, 'betapoints') && isfield(args, 'betares'))
        error('Specify either ''betapoints'' or ''betares'', but not both');
    end
    
    if (length(args.alpharange) ~= 2)
        error('Argument for ''alpharange'' must be a two element vector');
    end
    if (length(args.betarange) ~= 2)
        error('Argument for ''betarange'' must be a two element vector');
    end
    
    if isfield(args, 'betares')
        args.betapoints = (betaspan / args.betares) + 1;
        if (ceil(args.betapoints) ~= args.betapoints)
            error('Beta range and beta resolution are not an integer multiple')
        end
    else
        args.betares = betaspan / (args.betapoints-1);
    end

    % Orientation
    if (length(args.orient) ~= 3)
        error('Argument for ''orient'' must be a three element vector [yaw pitch roll]');
    end 
    
    % Measurement distance
    if ~isfield(args, 'mdist')
        fprintf('Assuming default measurement distance of 1.00 m');
        args.mdist = 1;
    end 
       
    % Reference
    if ~isfield(args, 'reference')
        fprintf('Setting reference to 1.0 (0 dB)');
        args.reference = 1;
    end 
    
    % Quantization
    if ~isfield(args, 'quantization'), args.quantization = 'int16'; end;
    if isfield(args, 'quantization')
        args.quantization = lower(args.quantization);
        switch args.quantization
            case 'int16'
                quantizationStr = '16-bit signed integer';
                quantizationType = 0; % DAFF_INT16

            case 'int24'
                quantizationStr = '24-bit signed integer';
                quantizationType = 1; % DAFF_INT24
                
            case 'float32'
                quantizationStr = '32-bit floating point';
                quantizationType = 2; % DAFF_FLOAT32

            otherwise
                error(['Invalid quantization (' args.quantization ')']);
        end
    end  
    
    if strcmp(args.content, 'IR')
        % Validation for IR content
    
        % Zero-threshold (default value)
        if ~isfield(args, 'zthreshold'), args.zthreshold = -inf; end;
        zthreshold_value = 10^(args.zthreshold/20);    
    end
    
    % Default value for base path
    if ~isfield(args, 'basepath'), args.basepath = ''; end;
    % DEBUG: disp( args );

    % Print a summary of the information
    
    fprintf('--= Summary =--\n');
    fprintf('Content type:       \t%s\n', contentStr);
    fprintf('Num channels:       \t%d\n', args.channels);
    fprintf('Num alpha points:   \t%d\n', args.alphapoints);
    fprintf('Alpha range:        \t[%0.1f°, %0.1f°]\n', alphastart, alphaend);
    fprintf('Alpha resolution:   \t%0.1f°\n', args.alphares);

    fprintf('Num beta points:   \t%d\n', args.betapoints);
    fprintf('Beta range:        \t[%0.1f°, %0.1f°]\n', betastart, betaend);
    fprintf('Beta resolution:   \t%0.1f°\n', args.betares);
    
    fprintf('Measurement dist.: \t%0.2f m\n', args.mdist);
    fprintf('Reference value:   \t%+0.1f dB\n', args.reference);

    fprintf('Orientation:       \t(Y%+0.1f°, P%+0.1f°, R%+0.1f°)\n', ...
                 args.orient(1), args.orient(2), args.orient(3));
    
    if strcmp(args.content, 'IR')
        fprintf('Quantization:      \t%s\n', quantizationStr);
        fprintf('Zero threshold:    \t%+0.1f dB (%0.6f)\n', args.zthreshold, zthreshold_value);
    end
    
    if strcmp(args.content, 'MS')
        % Nothing yet ...
    end    
    
    % +------------------------------------------------+
    % |                                                |
    % |   Analysis of the input data                   |
    % |                                                |
    % +------------------------------------------------+
      
    %
    %  Here we analyse all of the input data.
    %  It is ensured that all datasets have equal properties.
    %  The first dataset (for impulse responses) determines
    %  global properties (samplerate, filterlength)
    %
    
    props = struct();
    props.numRecords = 0;
    props.globalPeak = 0;
    props.eff_coeffs = 0;
    
    % Generate a list of all input files
    x = cell(args.alphapoints, args.betapoints);
    for b=1:args.betapoints
        beta = betastart + (b-1)*args.betares;
        
        for a=1:args.alphapoints
            alpha = alphastart + (a-1)*args.alphares;
          
            % --= Impulse responses =--
            
            if strcmp(args.content, 'IR') 
                % Get the data
                [data, samplerate] = args.datafunc(alpha, beta, args.basepath);
                [channels, filterlength] = size(data);

                if (class(data) ~= 'double')
                    error( sprintf('Dataset (A%0.1f°, B%0.1f°): Data function must deliver double values') );
                end
                
                if isfield(props, 'samplerate')
                    if (samplerate ~= props.samplerate)
                        error( sprintf('Dataset (A%0.1f°, B%0.1f°): Sampling rate does not match', alpha, beta) );
                    end

                    if (channels ~= args.channels)
                        error( sprintf('Dataset (A%0.1f°, B%0.1f°): Number of channels does not match', alpha, beta) );
                    end

                    if (filterlength ~= props.filterlength)
                       error( sprintf('Dataset (A%0.1f°, B%0.1f°): Filter length does not match', alpha, beta) );
                    end
                else
                    % Now set the global properties, if they have not been set yet
                    props.samplerate = samplerate;
                    props.filterlength = filterlength;
                    props.elementsPerRecord = filterlength;

                    % Check filter length for 16-byte alignment
                    if (mod(filterlength, 4) ~= 0)
                        error( sprintf('Dataset (A%0.1f°, B%0.1f°): Filter length is not a multiple of 4 (this is required for memory alignment)', alpha, beta) );
                    end

                    fprintf('Global properties: Sampling rate = %d Hz, filter length = %d\n',...
                            props.samplerate, props.filterlength);
                end

                % Determine the peak value
                peak = max(max(abs(data)));
                props.globalPeak = max([props.globalPeak peak]);

                % Determine effective ranges of the filters
                % Important: We use C-indexes here (starting with 0)
                
                if (zthreshold_value == 0)
                    % No efficient storage => Full impulse responses
                    offsets = zeros(1, args.channels);
                    elengths = ones(1, args.channels)*filterlength;
                else
                    offsets = zeros(1, args.channels);
                    elengths = ones(1, args.channels)*filterlength;
                    
                    % Determine the effective bounds
                    for c=1:args.channels
                        [lwr, upr] = daff_effective_bounds(data(c,:), zthreshold_value);
                        
                        if (lwr == -1)
                            % No bounds. Store everything.
                            offsets(c) = 0;
                            elengths(c) = filterlength;
                        else
                            % Keep the offset and length a modulo of 4 (16-byte alignment)
                            % (Note: lwr-1 => switch from Matlab indexing to C-indexing)
                            elen = upr-lwr+1;
                            offsets(c) = daff_lwrmul(lwr-1, 4);
                            elengths(c) = daff_uprmul(elen, 4);
                        end
                    end
                end     
                        
                % Update savings by hidden zeros
                props.eff_coeffs = props.eff_coeffs + sum(elengths);
                
                % Update filter offsets and effective lengths
                if isfield(props, 'minFilterOffset')
                    props.minFilterOffset = min([props.minFilterOffset min(offsets)]);
                    props.maxEffectiveFilterLength = max([props.maxEffectiveFilterLength max(elengths)]);
                else
                    props.minFilterOffset = min(offsets);
                    props.maxEffectiveFilterLength = max(elengths);
                end
                
                x{a,b} = struct('peak', peak, ...
                                'offsets', offsets, ...
                                'elengths', elengths); 
                            
                % Discard the data
                clear data;
                            
            end
            
            % --= Magnitude spectra =--
            
            if strcmp(args.content, 'MS') 
                % Get the data
                [freqs, data] = args.datafunc(alpha, beta, args.basepath);
                [channels, numfreqs] = size(data);

                if (class(data) ~= 'double')
                    error( sprintf('Dataset (A%0.1f°, B%0.1f°): Data function must deliver double values') );
                end
                
                if isfield(props, 'freqs')
                    if (freqs ~= props.freqs)
                        error( sprintf('Dataset (A%0.1f°, B%0.1f°): Frequency support does not match', alpha, beta) );
                    end

                    if (channels ~= args.channels)
                        error( sprintf('Dataset (A%0.1f°, B%0.1f°): Number of channels does not match', alpha, beta) );
                    end
                else
                    % Checks on the frequency support
                    if (min(freqs) <= 0)
                        error( sprintf('Dataset (A%0.1f°, B%0.1f°): Support frequencies must be greater zero', alpha, beta) );
                    end;
            
                    if (sort(freqs) ~= freqs)
                        error( sprintf('Dataset (A%0.1f°, B%0.1f°): Support frequencies must be stricly increasing', alpha, beta) );
                    end   
        
                    if (length(unique(freqs)) ~= length(freqs))
                        error( sprintf('Dataset (A%0.1f°, B%0.1f°): Support frequencies must be unique', alpha, beta) );
                    end  
                    
                    % Now set the global properties, if they have not been set yet
                    props.numfreqs = numfreqs;
                    props.freqs = freqs;
                    props.elementsPerRecord = length(freqs);

                    fprintf('Global properties: Number of frequencies = %d\n', numfreqs);
                end

                % Important: Negative magnitudes are forbidden
                if (min(min(data)) < 0)
                    error( sprintf('Dataset (A%0.1f°, B%0.1f°): Contains negative magnitudes', alpha, beta) );
                end
                
                % Determine the peak value
                peak = max(max(data));
                props.globalPeak = max([props.globalPeak peak]);
                
                x{a,b} = struct('peak', peak ); 
                            
                % Discard the data
                clear data; 
            end
            
            props.numRecords = props.numRecords + 1;
        end
    end
        
    fprintf('Global peak: %+0.1f dB (%0.6f)\n', 20*log10(props.globalPeak), props.globalPeak);
  
    if strcmp(args.content, 'IR') 
        % Calculate to overall storage saving
        props.total_coeffs = args.channels * props.numRecords * filterlength;
        props.zsavings = double(props.total_coeffs - props.eff_coeffs) / double(props.total_coeffs) * 100;
        
        fprintf('Minimum filter offset: %d\n', props.minFilterOffset);
        fprintf('Maximum effective filter length: %d\n', props.maxEffectiveFilterLength);
        fprintf('Storage space saved: %0.1f%%\n', props.zsavings);
    end  
    
    % +------------------------------------------------+
    % |                                                |
    % |   Writing of the output file                   |
    % |                                                |
    % +------------------------------------------------+
      
    % Current version = 0.1
    FileFormatVersion = 0100;
    
    % Important! 'l' -> little endian (DAFF files are always little endian)
    fid = fopen(args.filename, 'wb', 'l'); 
    fpos = struct();
    
    %
    %  1st step: Write the file header
    %
   
    if (length(args.metadata) > 0)
        iNumFileBlocks = 5;
    else
        iNumFileBlocks = 4;
    end

    fwrite(fid, 'FW', 'char');
    fwrite(fid, FileFormatVersion, 'int32');
    fwrite(fid, iNumFileBlocks, 'int32');
    
    % File block entries
    % Note: Remember the positions for later data insertion
    
    % Main header (FILEBLOCK_DAFF1_MAIN_HEADER = 0x0001)
    fwrite(fid, hex2dec('0001'), 'int32');
    fpos.MainHeaderOffset = ftell(fid);
    fwrite(fid, 0, 'uint64');
    fpos.MainHeaderSize = ftell(fid);
    fwrite(fid, 0, 'uint64');
    
    % Content header (FILEBLOCK_DAFF1_CONTENT_HEADER = 0x0002)
    fwrite(fid, hex2dec('0002'), 'int32');
    fpos.ContentHeaderOffset = ftell(fid);
    fwrite(fid, 0, 'uint64');
    fpos.ContentHeaderSize = ftell(fid);
    fwrite(fid, 0, 'uint64');
    
    % Record descriptor block (FILEBLOCK_DAFF1_RECORD_DESC = 0x0003)
    fwrite(fid, hex2dec('0003'), 'int32');
    fpos.RecordDescOffset = ftell(fid);
    fwrite(fid, 0, 'uint64');
    fpos.RecordDescSize = ftell(fid);
    fwrite(fid, 0, 'uint64');
    
    % Data block (FILEBLOCK_DAFF1_DATA  = 0x0004)
    fwrite(fid, hex2dec('0004'), 'int32');
    fpos.DataOffset = ftell(fid);
    fwrite(fid, 0, 'uint64');
    fpos.DataSize = ftell(fid);
    fwrite(fid, 0, 'uint64');
    
    if (length(args.metadata) > 0)
        % Metadata block (FILEBLOCK_DAFF1_METADATA = 0x0005)
        fwrite(fid, hex2dec('0005'), 'int32');
        fpos.MetadataOffset = ftell(fid);
        fwrite(fid, 0, 'uint64');
        fpos.MetadataSize = ftell(fid);
        fwrite(fid, 0, 'uint64');
    end
    
    %
    %  2nd step: Write the main header
    %
   
    MainHeaderOffset = ftell(fid);
    
    fwrite(fid, contentType, 'int32');
    fwrite(fid, quantizationType, 'int32');
    fwrite(fid, args.channels, 'int32');
    fwrite(fid, props.numRecords, 'int32');
    fwrite(fid, props.elementsPerRecord, 'int32');
    fwrite(fid, args.mdist, 'float32');

    fwrite(fid, args.alphapoints, 'int32');
    fwrite(fid, alphastart, 'float32');
    fwrite(fid, alphaend, 'float32');

    fwrite(fid, args.betapoints, 'int32');
    fwrite(fid, betastart, 'float32');
    fwrite(fid, betaend, 'float32');

    fwrite(fid, args.orient(1), 'float32');
    fwrite(fid, args.orient(2), 'float32');
    fwrite(fid, args.orient(3), 'float32');
 
    MainHeaderSize = ftell(fid) - MainHeaderOffset;
    
    %
    %  3rd step: Write the content specific header
    %
    
    ContentHeaderOffset = ftell(fid);
 
    % --= Impulse responses =--
 
    if strcmp(args.content, 'IR') 
        fwrite(fid, props.samplerate, 'float32');
        fwrite(fid, args.reference, 'float32');
        fwrite(fid, props.minFilterOffset, 'int32');
        fwrite(fid, props.maxEffectiveFilterLength, 'int32');
    end
        
    % --= Magnitude spectra =--
          
    if strcmp(args.content, 'MS') 
        % Number of frequencies
        fwrite(fid, props.globalPeak, 'float32');
        fwrite(fid, props.numfreqs, 'int32');
        fwrite(fid, props.freqs, 'float32');
    end  

    ContentHeaderSize = ftell(fid) - ContentHeaderOffset;
    
    % Adjust 16-Byte alignment
    nfill = mod(ftell(fid),16);
    if (nfill > 0)
        for i=1:(16-nfill), fwrite(fid, 0, 'uint8'); end;
    end
        
    %
    %  4rd step: Write the record descriptor table
    %
    
	RecordDescOffset = ftell(fid);
    
    % Note: Here we do not write the actual descriptors, since they are
    %       unknown so far. We write placeholder and insert the data later...
    
    if strcmp(args.content, 'IR') 
        % Note: Each IR record desc is 4+4+8 Byte = 16 Byte
        fwrite(fid, zeros(1, 16*props.numRecords*args.channels, 'uint8'), 'uint8');
    end
    
    if strcmp(args.content, 'MS') 
        % Note: Each MS record desc is 8 Byte
        fwrite(fid, zeros(1, 8*props.numRecords*args.channels, 'uint8'), 'uint8');
    end
    
    RecordDescSize = ftell(fid) - RecordDescOffset;
    
    % Adjust 16-Byte alignment
    nfill = mod(ftell(fid),16);
    if (nfill > 0)
        for i=1:(16-nfill), fwrite(fid, 0, 'uint8'); end;
    end
         
    %
    %  5th step: Write the data itself
    %
    
    DataOffset = ftell(fid);
 
    if strcmp(args.content, 'IR') 
        
        for b=1:args.betapoints
            beta = betastart + (b-1)*args.betares;
            
            for a=1:args.alphapoints
                alpha = alphastart + (a-1)*args.alphares;
                
                % Get the data (2nd time)
                [data, samplerate] = args.datafunc(alpha, beta, args.basepath);
                [channels, filterlength] = size(data);

                if (class(data) ~= 'double')
                    error( sprintf('Dataset (A%0.1f°, B%0.1f°): Data function must deliver double values') );
                end
 
                % Clipping check
                peak = max(max(abs(data)));
                if (peak > 1)
                    warnning( sprintf('Dataset (A%0.1f°, B%0.1f°): Clipping occured (peak %0.3f)', alpha, beta, peak) );
                end
                   
                x{a,b}.dataOffset = zeros(1, args.channels);
                for c=1:args.channels
                    % Remember the offset of this records/channels data
                    % Important: Relative to the beginning of the data block
                    x{a,b}.dataOffset(c) = ftell(fid) - DataOffset;
                    
                    % Effective boundaries (Matlab indices)
                    i1 = x{a,b}.offsets(c) + 1;
                    i2 = i1 + x{a,b}.elengths(c) - 1;
                    
                    switch args.quantization
                    case 'int16'
                        % Dynamic range: 2^15-1 = 32767
                        cdata = int16( data(c,i1:i2)*32767 );
                        fwrite(fid, cdata, 'int16');

                    case 'int24'
                        % Dynamic range: 2^23-1 = 8388607
                        cdata = int32( data(c,i1:i2)*8388607 );
                        fwrite(fid, cdata, 'int24');

                    case 'float32'
                        fwrite(fid, data(c,i1:i2), 'float32');
                    end
                end
            end
        end
    end
    
    if strcmp(args.content, 'MS') 
        
        for b=1:args.betapoints
            beta = betastart + (b-1)*args.betares;
            
            for a=1:args.alphapoints
                alpha = alphastart + (a-1)*args.alphares;
                
                % Get the data (2nd time)
                [freqs, data] = args.datafunc(alpha, beta, args.basepath);
                [channels, numfreqs] = size(data);

                if (class(data) ~= 'double')
                    error( sprintf('Dataset (A%0.1f°, B%0.1f°): Data function must deliver double values') );
                end
 
                % Clipping check
                peak = max(max(data));
                if (peak > 1)
                    warning( sprintf('Dataset (A%0.1f°, B%0.1f°): Clipping occured (peak %0.3f)', alpha, beta, peak) );
                end
                   
                x{a,b}.dataOffset = zeros(1, args.channels);
                for c=1:args.channels
                    % Remember the offset of this records/channels data
                    % Important: Relative to the beginning of the data block
                    x{a,b}.dataOffset(c) = ftell(fid) - DataOffset;
   
                    fwrite(fid, data(c,:), 'float32');
                end
            end
        end
    end

    DataSize = ftell(fid) - DataOffset;
        
    %
    %  6th step: Write the metadata
    %
    
    if (length(args.metadata) > 0)
        MetadataOffset = ftell(fid);
        daff_write_metadata(fid, args.metadata);
        MetadataSize = ftell(fid) - MetadataOffset;
    end
    
    %
    %  7th step: Update the record descriptors
    %
    
    fseek(fid, RecordDescOffset, 'bof');
    
    if strcmp(args.content, 'IR') 
        for b=1:args.betapoints
            for a=1:args.alphapoints
                for c=1:args.channels
                    fwrite(fid, x{a,b}.offsets(c), 'int32');
                    fwrite(fid, x{a,b}.elengths(c), 'int32');
                    fwrite(fid, x{a,b}.dataOffset(c), 'uint64');
    
                    % DEBUG: fprintf('Data offset alpha = %d, beta = %d, channel %d = %d\n', a, b, c, x{a,b}.dataOffset(c));
                end
            end
        end
    end
    
    if strcmp(args.content, 'MS') 
        for b=1:args.betapoints
            for a=1:args.alphapoints
                for c=1:args.channels
                    fwrite(fid, x{a,b}.dataOffset(c), 'uint64');
                end
            end
        end
    end
    
    %
    %  8th step: Finally insert offsets and sizes into the file header
    %
  
    fseek(fid, fpos.MainHeaderOffset, 'bof');
    fwrite(fid, MainHeaderOffset, 'uint64');
    
    fseek(fid, fpos.MainHeaderSize, 'bof');
    fwrite(fid, MainHeaderSize, 'uint64');
    
    fseek(fid, fpos.ContentHeaderOffset, 'bof');
    fwrite(fid, ContentHeaderOffset, 'uint64');
    
    fseek(fid, fpos.ContentHeaderSize, 'bof');
    fwrite(fid, ContentHeaderSize, 'uint64');
    
    fseek(fid, fpos.RecordDescOffset, 'bof');
    fwrite(fid, RecordDescOffset, 'uint64');
    
    fseek(fid, fpos.RecordDescSize, 'bof');
    fwrite(fid, RecordDescSize, 'uint64');
    
    fseek(fid, fpos.DataOffset, 'bof');
    fwrite(fid, DataOffset, 'uint64');    
    
    fseek(fid, fpos.DataSize, 'bof');
    fwrite(fid, DataSize, 'uint64');    
    
    if (length(args.metadata) > 0)
        fseek(fid, fpos.MetadataOffset, 'bof');
        fwrite(fid, MetadataOffset, 'uint64');
    
        fseek(fid, fpos.MetadataSize, 'bof');
        fwrite(fid, MetadataSize, 'uint64');
    end
    
    fclose(fid);

    % -----------------------------------------------------
    
    % Some more information
    fprintf('--= Summary =---------------------------------\n\n', ContentHeaderSize);
    fprintf('Content header size = %d bytes\n', ContentHeaderSize);
    fprintf('Record descriptor size = %d bytes\n', RecordDescSize);
    fprintf('Data size = %d bytes\n', DataSize);
    if (length(args.metadata) > 0)
        fprintf('Metadata size = %d bytes\n', MetadataSize);
    end
    fprintf('\n----------------------------------------------\n\n', ContentHeaderSize);
    
    % What we all been waiting for...
    fprintf('DAFF file ''%s'' successfully written\n', args.filename);
end

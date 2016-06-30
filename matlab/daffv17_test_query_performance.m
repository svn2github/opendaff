%
%  OpenDAFF
%
%  File:    testQueryPerformance.m
%  Purpose: Benchmark runtimes for querying records
%  Author:  Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
%
%  $Id: testQueryPerformance.m,v 1.2 2010/03/04 13:49:26 fwefers Exp $
%

filename = ' << Enter your filename here >> ';
h = DAFFv17('open', filename);

props = DAFFv17('getProperties', h)
metadata = DAFFv17('getMetadata', h);

fprintf('Querying %d all records by index ...\n\n', props.numRecords);
tic;
for i=1:props.numRecords
    data = DAFFv17('getRecordByIndex', h, i);
end
t = toc;
fprintf('Total runtime: %0.3f ms\n', t*1000);
fprintf('Runtime/query: %0.3f us\n\n', t*1000000 / props.numRecords);

fprintf('Querying %d random records by nearest neighbour ...\n\n', props.numRecords);
tic;
for b=1:props.betaPoints
    for a=1:props.alphaPoints
        data = DAFF('getNearestNeighbour', h, 'object', rand()*360, rand*180);
    end
end
t = toc;
fprintf('Total runtime: %0.3f ms\n', t*1000);
fprintf('Runtime/query: %0.3f us\n\n', t*1000000 / props.numRecords);


DAFFv17('close', h);
clear DAFFv17;

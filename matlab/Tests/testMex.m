close all;
clear all;

h = DAFF('open', 'ir.daff');

props = DAFF('getProperties', h)
metadata = DAFF('getMetadata', h);

% for i=1:props.alphaPoints
%     DAFF('getRecordCoords', h, 'object', i)
% end

% Impulse response
% [data1] = DAFF('getNearestNeighbour', h, 'object', 0, 0);
% [data1] = DAFF('getRecordByIndex', h, 1);
% figure;
% hold on
% plot(data1(1,1:512), 'r')
% plot(data1(2,1:512), 'k')

[data2] = DAFF('getNearestNeighbour', h, 'object', 170, 0);
[data2] = DAFF('getRecordByIndex', h, 9);
figure;
hold on
plot(data2(1,1:512), 'r')
plot(data2(2,1:512), 'k')


% Directivity
% 
% figure;
% hold on
% for i=1:9
%     [data] = DAFF('getNearestNeighbour', h, 'object', 0, i*10);
%     semilogx(props.freqs, data, 'r')
% end
% 
DAFF('close', h);
clear DAFF;

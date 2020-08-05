% function pruebas
%%
clc;clear all;
Fs = 44100;
fc = 60;
fs = fc/(Fs/2);
fp = fc+3;
fp = fp/(Fs/2);
[n wn] = buttord( fp, fs, 0.5, 80 );
[b, a] = butter(5,wn);
%%


[ H, F ] = freqz( b, a, 512,  1 );
F = Fs * F;
H = abs(H);
plot( F, H );
axis( [0 Fs/256 0 (max(H)+1)] );

data = sprintf('\nconst int n = %i;\nconst double b[%i] = { ', length(b), length(b) );
for id = 1:length(b)
    if id==length(b)
        data = sprintf('%s%5.16f};', data, b(id) );
        break;
    else
        data = sprintf('%s%5.16f, ',  data, b(id) );
    end
end


data = sprintf('%s\nconst double a[%i] = {', data, length(a) );
for id = 1:length(a)
    if id==length(a)
        data = sprintf('%s%5.16f};\n\n\n', data, 0 );
        break;
    else
        data = sprintf('%s%5.16f, ',  data, a(id+1) );
    end
end
clipboard('copy', data)


return
%%
clc;clear all;
n = 2048;
Fs = 44100;

fc = 40;
fs = fc/(Fs/2);
fp = fc+5;
fp = fp/(Fs/2);
    
FcHz = fc;
Fc   = FcHz/(Fs/2);
Wn = Fc;
% b = fir1( n, Wn, 'stop' );
b = fir1( n, Wn, 'High' );

data = sprintf('\nconst int n = %i;\nconst double b[n] = { ', length(b) );
for id = 1:length(b)
    if id==length(b)
        data = sprintf('%s%5.32f};\n\n\n', data, b(id) );
        break;
    else
        data = sprintf('%s%5.32f, ',  data, b(id) );
    end
end
clipboard('copy', data)

%%
try %#ok<TRYNC>
    stop( player );
    delete(player)
end

clc;clear all;
Fs = 48000;
FsN = Fs/2;
Wp = 50/FsN;
Ws = 500/FsN;
[n, Wn] = buttord( Wp, Ws, 20, 60 );



[b,a] = butter( n, Wn );
clc;

fprintf('\nconst int n = %i;\nconst double b[n] = { ',...
                    max([length(b), length(a)]) );
for id = 1:max([length(b), length(a)])
    if id==length(b)
        fprintf('%5.32f};', b(id) );
        break;
    else
        fprintf('%5.32f, ', b(id) );
    end
end
fprintf('\nconst double a[n] = { ');
for id = 1:max([length(b), length(a)])
    if id==length(a)
        fprintf('%5.32f};\n\n\n\n\n\n', a(id) );
        break;
    else
        fprintf('%5.32f, ', a(id) );
    end
end

return
freqz( b, a, 512, 1000 ); 
title('n=5 Butterworth Lowpass Filter')


[y Fs] = wavread( 'X:\SegAll\CONOCIMIENTO\UNIVERSIDAD\TRATAMIENTO\PROYECTO\Abrete.wav' );
y = y( 1:length(y)/1);
Ts = 1/Fs;% wavplay( y, Fs );
Gz = tf( b, a, Ts, 'z' );
t = Ts * (0:(length( y )-1));
drawnow
tic;
y1 = filtfilt( b, a, y );
y1 = y1/max( abs(y1) );
% disp( 'Tiempo, ' )
% disp( toc )


% 
% %%AO
% player = audioplayer(y1, Fs);
% play(player);


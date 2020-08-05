function [yhi, b, a]= hipass( y, Fs, FcHz, dcB )
Fc = FcHz/(Fs/2);
fs = (FcHz - 10)/(Fs/2);
b = fir1( 256, Fc, 'High' );
[n wn] = buttord( Fc, fs, 0.5, dcB );
[b, a] = butter( 5, wn, 'High' );


if size(y,2)==2
    channel_1 = filtfilt( b, a, y(:,1) );
    channel_2 = filtfilt( b, a, y(:,2) );
    yhi = [channel_1 channel_2 ];
    yhi(:,1) = yhi(:,1)/max( abs(yhi(:,1)) );
    yhi(:,2) = yhi(:,2)/max( abs(yhi(:,2)) );
else
    channel_1 = filtfilt( b, a, y(:,1) );
    ylow = channel_1;
end


return

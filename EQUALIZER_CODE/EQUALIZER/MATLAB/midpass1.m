function [yhi, b, a]= midpass1( y, Fs, FcHz )
Fc = FcHz/(Fs/2);
fs = ([(FcHz(1) - 10) (FcHz(2) + 50)])/(Fs/2);
[n wn] = buttord( Fc, fs, 0.5, 80 );
[b, a] = butter( 5, wn );



if size(y,2)==2
    channel_1 = filtfilt( b, a, y(:,1) );
    channel_2 = filtfilt( b, a, y(:,2) );
    yhi = [channel_1 channel_2 ];
    yhi(:,1) = yhi(:,1)/max( abs(yhi(:,1)) );
    yhi(:,2) = yhi(:,2)/max( abs(yhi(:,2)) );
else
    channel_1 = filtfilt( b, a, y(:,1) );
    yhi = channel_1;
end


return

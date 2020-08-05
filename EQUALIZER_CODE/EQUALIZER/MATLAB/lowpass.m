function ylow= lowpass( y, Fs, FcHz, dcB, NOrd )
Fc = FcHz/(Fs/2);
fs = (FcHz + 10)/(Fs/2);
[n wn] = buttord( Fc, fs, 0.5, dcB );
[b, a] = butter( NOrd, wn, 'Low' );
fprintf( '\na = \n' );
for u=1:length( a )
    fprintf( '%5.32f\n', a(u) );
end
fprintf( '\nb = \n' );
for u=1:length( b )
    fprintf( '%5.32f\n', b(u) );
end


length( b )

if size(y,2)==2
    channel_1 = filtfilt( b, a, y(:,1) );
    channel_2 = filtfilt( b, a, y(:,2) );
    ylow = [channel_1 channel_2 ];
    ylow(:,1) = ylow(:,1)/max( abs(ylow(:,1)) );
    ylow(:,2) = ylow(:,2)/max( abs(ylow(:,2)) );
else
    ylow = filtfilt( b, a, y(:,1) );
    ylow = ylow/max( abs(ylow) );
end

return

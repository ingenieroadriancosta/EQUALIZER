% function pruebas
clc;clear all;

[ y Fs ] = wavread( 'audio.wav' );
yNT = y;%(25*Fs:30*Fs,:);
yNT =  yNT/max( abs(yNT) );

ylow= lowpass( yNT, Fs, 200, 3, 3 );

ylow = ylow/max( abs(ylow) );
ylow = 10 * ylow + y;
ylow = ylow/max( abs(ylow) );

for ind=1:length( ylow )
    if abs( ylow(ind) )>0.9
        ylow(ind) = ylow(ind);
    end
end

plot( ylow )
drawnow
wavplay( ylow, Fs );
 

% yN = [ yN yN ];
% ylow= lowpass( yNT, Fs, 300, 60 );
% yhi= hipass( yNT, Fs, 5000, 60 );
% yN = ( 0.9*ylow + 0.4*yN );
% yN = ylow + yhi;

% yN = yN(1:10*Fs);
% yN(:,1) = 0.9 * yN(:,1)/max( abs(yN(:,1)) );
% yN(:,2) = 0.9 * yN(:,2)/max( abs(yN(:,2)) );


% ylow = ylow/max( abs(ylow) );
%  wavplay( ylow(25*Fs:40*Fs,:), Fs );

% wavplay( yNT(25*Fs:40*Fs,:), Fs );
% wavplay( yN(25*Fs:40*Fs,:), Fs );


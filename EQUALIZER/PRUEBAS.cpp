BOOL PRUEBAS(void){
    SetWindowTextF( HW_Info, (char*)"PRUEBAS...");
    WCLS.PlaySong( (char*)"X:\\SegAll\\CONOCIMIENTO\\UNIVERSIDAD\\TRATAMIENTO\\PROYECTO\\Abrete.wav");
    //
    WCLS.ReSetVal();
    WCLS.NormBufY();
    double* y = WCLS.GetY();
    double* x = WCLS.GetX();
    int Size = WCLS.GetSize();


    const double bLPF[4] = { 0.0000000348544221, 0.0000001045632663, 0.0000001045632663, 0.0000000348544221};
    const double aLPF[4] = {-2.9869054238176194, 2.9738964412704219, -0.9869907386174257, 0.0000000000000000};

    /*
    const double bk[6] = { 0.9967200902731339, -4.9836004513656693, 9.9672009027313386, -9.9672009027313386, 4.9836004513656693, -0.9967200902731339};
    const double ak[6] = {-4.9934294008818014, 9.9737391847402161, -9.9606411051342398, 4.9737822596299432, -0.9934509383540842, 0.0000000000000000};
    //*/





const double bk[6] = { 0.9987228574842059, -4.9936142874210292, 9.9872285748420584, -9.9872285748420584, 4.9936142874210292, -0.9987228574842059};
const double ak[6] = {-4.9974440825868189, 9.9897795963996767, -9.9846742910990578, 4.9897861233476180, -0.9974473460614176, 0.0000000000000000};














    int n = 0;
    double* b = NULL;
    double* a = NULL;
    BOOL Kareoke = TRUE;
    IF Kareoke==TRUE THEN
        n = 6;
        b = (double*)&bk[0];
        a = (double*)&ak[0];
    ELSE
        n = 4;
        b = (double*)&bLPF[0];
        a = (double*)&aLPF[0];
    ENDIF;






double VR1 = 0;
double VR2 = 0;

SetWindowTextF( HW_Info, (char*)"Procesando...");
tic
    for(int i=n;i<Size;i++){
        VR1 = 0;
        VR2 = 0;
        for(int ix=0;ix<n;ix++){
            VR1 = VR1 + b[ix] * x[2*i-2*ix];
            VR2 = VR2 + a[ix] * y[2*i-2*ix-2];
        }
        y[2*i] = VR1 - VR2;
    }
    IF Kareoke==TRUE THEN
        for(int i=0;i<Size;i++){
            y[2*i+1] = y[2*i+1] - y[2*i];
            y[2*i] = y[2*i+1];
            //y[2*i+1] = y[2*i];
        }
    ELSE
        for(int i=0;i<Size;i++){
            y[2*i+1] = y[2*i];
        }
    ENDIF;
    //*/
    WCLS.NormBufY();
    WCLS.FillSBufY();
    //*/
    SetWindowTextF( HW_Info, (char*)"Ok\nToc:\n%f", toc);
    return TRUE;
}

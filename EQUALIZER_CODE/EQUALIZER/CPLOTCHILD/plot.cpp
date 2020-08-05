BOOL CPLOTCHILD::plot( void ){
    if( UCBuff!=NULL ){
        plotPUC();
        return TRUE;
    }
    FillGrid();
    SetBitmapBits( HBitmap, 4 * WFR * HFR, &RgbBuf[0] );
    SendMessage( HWPlot, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBitmap );
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
BOOL CPLOTCHILD::plotV( int ESM ){
    InPlotV = TRUE;
    DeleteObject( HBitmap );
    HBitmap = NewBitmap( WFR, HFR );
    ZeroMemory( &RgbBuf[0], 4 * WFR * HFR - 1 );
    if( !plot() ){
        FillGrid();
        SetBitmapBits( HBitmap, 4 * WFR * HFR, &RgbBuf[0] );
        MoveWindow( HWPlot, 1, 1, WFR, HFR, TRUE );
        SendMessage( HWPlot, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBitmap );
    }
    InPlotV = FALSE;
    return TRUE;
}






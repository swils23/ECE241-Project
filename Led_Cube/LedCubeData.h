#ifndef LedCubeData_H
#define LedCubeData_H

// Establish constants for LedCube 
#define LEDCUBE_NUMBER_PLANES  4
#define LEDCUBE_NUMBER_ROWS    4
#define LEDCUBE_NUMBER_COLUMNS 4

// Establish data for LedCube
// Variable representing led's state (on/off => 0/1)
// for each plane.
int LedCube_Planes[LEDCUBE_NUMBER_PLANES] = {-1};

// variable representing plane that is currently on.
int LedCube_CurrentPlane = 0;

// Support routine to move to next plane, returning
// the bit pattern for the next plane to be displayed.
int LedCube_NextPlane(){ 
    // Move to next plane.
    LedCube_CurrentPlane++;
    // Check for wrap.
    if( LedCube_CurrentPlane >= LEDCUBE_NUMBER_PLANES )
        LedCube_CurrentPlane = 0;

    // Send back the current bit pattern.
    return LedCube_Planes[LedCube_CurrentPlane];
    // BIT PATTERN FLIPPED, for reversed cube.
}

// Initialization routine, sets all leds to off.
void LedCube_ClearData(){
    // Loop through the planes.
    for( int k = 0; k < LEDCUBE_NUMBER_PLANES; k++ )
        LedCube_Planes[k] = 0xffff;
           // Note a bit being 1 means off.

}

// This function turns on the led at r,c,p 
// (row, column, plane.
void LedCube_SetLed( int r, int c, int p ){
    // make copy of r,c,p
    int plane = p, row = r, col = c;
    
    // Error Check r,c,p
    if( plane < 0 ) plane = 0;
    if( plane >= LEDCUBE_NUMBER_PLANES )
        plane = LEDCUBE_NUMBER_PLANES - 1;
    if( row < 0 ) row = 0;
    if( row >= LEDCUBE_NUMBER_ROWS )
        row = LEDCUBE_NUMBER_ROWS - 1;
    if( col < 0 ) col = 0;
    if( col >= LEDCUBE_NUMBER_COLUMNS )
        col = LEDCUBE_NUMBER_COLUMNS - 1;

    // force the bit low in LedCube_Planes, turning that led on.
    LedCube_Planes[plane] 
         &= ~(1 << (row*LEDCUBE_NUMBER_COLUMNS+col));

}

// This function will turn off the led at r,c,p
void LedCube_ClearLed( int r, int c, int p ){
    // Make a copy of r,c,p
    int plane = p, row = r, col = c;
    // Error check r,c,p
    if( plane < 0 ) plane = 0;
    if( plane >= LEDCUBE_NUMBER_PLANES ) 
        plane = LEDCUBE_NUMBER_PLANES - 1;
    if( row < 0 ) row = 0;
    if( row >= LEDCUBE_NUMBER_ROWS )
        row = LEDCUBE_NUMBER_ROWS - 1;
    if( col < 0 ) col = 0;
    if( col >= LEDCUBE_NUMBER_COLUMNS ) 
        col = LEDCUBE_NUMBER_COLUMNS - 1;
    // force bit high at r,c,p turning it off
    LedCube_Planes[plane] 
         |= (1 << (row*LEDCUBE_NUMBER_COLUMNS+col));
         
}

#endif

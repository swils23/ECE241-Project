public class Pattern {
    int r, c, p, count;
    int[] x;
    Matrix m;

    Pattern(){
        m = new Matrix(4, 4, 4);
        r = 0;
        c = 0;
        p = 0;
        x = new int[]{0, 0, 0};
    }

    public void reset(){
        r = 0;
        c = 0;
        p = 0;
        count = 0;
        x = new int[]{0, 0, 0};
    }

    // lights up each led individually
    public void Mode1(){
        // clear data
        m.clearMatrix();
        // set data
        m.setLed(x);
        m.printMatrix();

        // generate data
        r++;
        if (r > 3){
            r = 0;
            c++;
        }
        if (c > 3){
            c = 0;
            p++;
        }
        if (p > 3){
            p = 0;
        }
        x = new int[]{r, c, p};
    }

    // lights up 4x4 squares on each plane, then rotates and lights up 4x4 squares on each column, then rotates and lights up 4x4 squares on each row
    public void Mode2(){
        m.printMatrix();
        m.clearMatrix();

        if (count < 4){
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 4; j++)
                    m.setLed(i, j, count);
            }
        } else if (count < 8){
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 4; j++)
                    m.setLed(i, count - 4, j);
            }
        } else if (count < 12){
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 4; j++)
                    m.setLed(count - 8, i, j);
            }
        }
        count++;
        if (count > 11){
            reset();
        }
    }

    // "snakes" through the matrix eventually lighting up the entire matrix, then unlights the entire matrix in the same fashion
    public void Mode3(){
        
        m.printMatrix();
        m.setLed(r, c, p);

        // ticker
        if (((p % 2 == 0) && (c % 2 == 0)) || (!(p % 2 == 0) && !(c % 2 == 0))) r++;
        else r--;

        // "snake" logic
        if (p % 2 == 0){ // even planes
            if (r > 3){
                r = 3;
                c++;
            }
            if (r < 0){
                r = 0;
                c++;
            }
            if (c > 3){
                c = 3;
                p++;
            }
        }else{ // odd planes
            if (r > 3){
                r = 3;
                c--;
            }
            if (r < 0){
                r = 0;
                c--;
            }
            if (c < 0){
                c = 0;
                p++;
            }
        }

        // TODO reset or snake back through turning leds off?
        if (p > 3){
            m.clearMatrix();
            reset();
        }

    }

    // checkerboard pattern
    public void Mode4(){

        m.clearMatrix();        
        for(int i = 0; i < 4; i++) // iterate planes
            for(int j = 0; j < 4; j++) // iterate rows
                for(int k = 0; k < 4; k++){ // iterate columns
                    if((i + j + k) % 2 == 0 && (count % 2 == 0)) m.setLed(k, j, i); // even cycle and even led position
                    if((i + j + k) % 2 == 1 && (count % 2 == 1)) m.setLed(k, j, i); // odd cycle and odd led position
                }
        count++;
        

        m.printMatrix();
        
    }

    // TODO random leds?



    private int ZeroRow  = 0, ZeroColumn  = 0;
    private void MoveZero()
    {
    ZeroColumn++;
    if ( ZeroColumn >= 4 )
    {
        ZeroColumn = 0;
        ZeroRow++;
        if ( ZeroRow >= 4 )
        ZeroRow = 0;
    }
    } // End of MoveZero

    private int OneRow   = 1, OneColumn   = 1;
    private void MoveOne()
    {
    OneColumn++;
    if ( OneColumn >= 4 )
    {
        OneColumn = 0;
        OneRow++;
        if ( OneRow >= 4 )
        OneRow = 0;
    }
    } // End of MoveOne

    private int TwoRow   = 2, TwoColumn   = 2;
    private void MoveTwo()
    {
    TwoColumn++;
    if ( TwoColumn >= 4 )
    {
        TwoColumn = 0;
        TwoRow++;
        if ( TwoRow >= 4 )
        TwoRow = 0;
    }
    } // End of MoveTwo

    private int ThreeRow = 3, ThreeColumn = 3;
    private void MoveThree()
    {
    ThreeColumn++;
    if ( ThreeColumn >= 4 )
    {
        ThreeColumn = 0;
        ThreeRow++;
        if ( ThreeRow >= 4 )
        ThreeRow = 0;
    }
    }  // End of MoveThree

    public void Mode0()
    {
    m.printMatrix();
    m.clearMatrix();
    MoveZero();
    m.setLed( ZeroRow,  ZeroColumn,  0 ); 
    MoveOne();
    m.setLed( OneRow,   OneColumn,   1 ); 
    MoveTwo();
    m.setLed( TwoRow,   TwoColumn,   2 ); 
    MoveThree();
    m.setLed( ThreeRow, ThreeColumn, 3 ); 
    }

 
}

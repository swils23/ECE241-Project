public class Pattern {
    int r, c, p;
    int[][][] x;

    Pattern(){
        r = 0;
        c = 0;
        p = 0;
        x = new int[4][4][4]; // 4x4x4 matrix
    }

    public int[] Mode1(){

        // clear data

        // set data

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
        return new int[]{r, c, p};
    }
 
}

public class Matrix {
    private int[][][] matrix;
    private int m1R, m1C, m1P;

    Matrix(int r, int c, int p){
        matrix = new int[r][c][p];

        m1R = 0;
        m1C = 0;
        m1P = 0;

    }

    public void setLed(int r, int c, int p){
        matrix[r][c][p] = 1;
    }

    public void setLed(int[] x){
        matrix[x[0]][x[1]][x[2]] = 1;
    }

    public void clearLed(int r, int c, int p){
        matrix[r][c][p] = 0;
    }

    public void clearLed(int[] x){
        matrix[x[0]][x[1]][x[2]] = 0;
    }

    public void clearMatrix(){
        for(int r = 0; r < matrix.length; r++){
            for(int c = 0; c < matrix[r].length; c++){
                for(int p = 0; p < matrix[r][c].length; p++){
                    matrix[r][c][p] = 0;
                }
            }
        }
    }

    public void printMatrix(){
        // for each row
        for (int r = 0; r < 4; r++){
            // for each plane
            for (int p = 0; p < 4; p++){
                // for each column
                for (int c = 0; c < 4; c++){
                    System.out.print(matrix[r][c][p]);
                    System.out.print(" ");
                }
                System.out.print("\t");
            }
            System.out.println("");
        }
    }
    






}

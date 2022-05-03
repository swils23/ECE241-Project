import java.util.*;
public class PatternVisualizer {
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);
        Matrix m = new Matrix(4, 4, 4);
        Pattern p = new Pattern();

        int[] x = {0, 0, 0};
        while (true){
            m.clearMatrix();
            m.setLed(x);
            m.printMatrix();
            
            x = p.Mode1();

            s.nextLine();
        }
       
        

    }
        






}
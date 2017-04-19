package etherPack0;

import java.awt.*;       // Using AWT layouts
import java.awt.event.*; // Using AWT event classes and listener interfaces
import java.io.IOException;
import java.net.SocketException;
import java.net.UnknownHostException;

import javax.swing.*;    // Using Swing components and containers
//import Pack;

public class PacketGUI extends JFrame {

   private JTextField dip;  // Use Swing's JTextField instead of AWT's TextField
   private JTextField sip;
   private JTextField pay;
   private JTextField out;
   private JButton submit;    // Using Swing's JButton instead of AWT's Button
   //private int count = 0;
 
   // Constructor to setup the GUI components and event handlers
   public PacketGUI() {
      // Retrieve the content-pane of the top-level container JFrame
      // All operations done on the content-pane
      Container cp = getContentPane();
      cp.setLayout(new GridLayout(4,2));   // The content-pane sets its layout
 
      cp.add(new JLabel("destination IP: "));
      dip = new JTextField(10);
	  cp.add(dip);
	  
	  cp.add(new JLabel("source IP: "));
	  sip = new JTextField(10);
	  cp.add(sip);
	  
	  cp.add(new JLabel("payload: "));
	  pay = new JTextField();
      cp.add(pay);
 
      submit = new JButton("Submit");
      cp.add(submit);
	  
	  out = new JTextField(10);
	  out.setEditable(false);
	  cp.add(out);
 
      // Allocate an anonymous instance of an anonymous inner class that
      //  implements ActionListener as ActionEvent listener
      submit.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent evt) {
            String dips = dip.getText();
			String sips = sip.getText();
			String pays = pay.getText();
			try { Pack.main(dips, sips, pays); }
			catch (UnknownHostException a) {}
			catch (SocketException b) {}
			catch (IOException c) { }
			out.setText("Sent!");
         }
      });
 
      setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  // Exit program if close-window button clicked
      setTitle("etherPack"); // "super" JFrame sets title
      setSize(300, 100);         // "super" JFrame sets initial size
      setVisible(true);          // "super" JFrame shows
   }
 
   // The entry main() method
   public static void main(String[] args) {
      // Run the GUI construction in the Event-Dispatching thread for thread-safety
      SwingUtilities.invokeLater(new Runnable() {
         @Override
         public void run() {
            new PacketGUI(); // Let the constructor do the job
         }
      });
   }

}

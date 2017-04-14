import java.io.IOException;
import java.net.*;
import java.util.Scanner;

public class Main {

	public static void main(String[] args) throws UnknownHostException, SocketException, IOException  {
		
		/* packet fields */
		byte[] payload = 		{ 66, 66, 69, 78, 66, 69, 78, 66, 69, 78 };
		byte[] sourceIP = 		{ (byte)198, (byte)168, (byte)1, (byte)15 };
		byte[] destinationIP =	{ (byte)198, (byte)168, (byte)1, (byte)15 };
		
		int destinationPort = 57;
		int byteCount = 0;			// expected number of bytes
		String seperatorString = "[.]+";
		char seperator = '.';
		
		// gather inputs
		Scanner input = new Scanner(System.in);
		System.out.println("Destination IP:\n");
		//System.out.println("Your current IP address is:\n");
		System.out.println(InetAddress.getLocalHost().getHostAddress());
		String newIP = input.next();
		System.out.println("Payload:\n");
		String newPayload = input.next();
		System.out.println("Source IP:\n");
		String newSourceIP = input.next();
		
		/* parse into a byte array */
		byteCount = countOccurrences(newIP, seperator) +1;
		
		String[] iptokens = newIP.split(seperatorString);
		for (int i = 0; i < iptokens.length; i++) {
			int test = Integer.parseInt(iptokens[i]);
			destinationIP[i] = (byte) (test & 0xFF);	// masking the int with FF allows us to use "unsigned" byte up to 255
		}
		
		String[] buffTokens = newPayload.split(seperatorString);
		for (int i = 0; i < buffTokens.length; i++) {
			int test = Integer.parseInt(buffTokens[i]);
			payload[i] = (byte) (test & 0xFF);
		}
		
		//With the tools I used currently I need to look into things more to edit sender IP
		String[] sIPtokens = newSourceIP.split(seperatorString);
		for (int i = 0; i < sIPtokens.length; i++) {
			int test = Integer.parseInt(sIPtokens[i]);
			sourceIP[i] = (byte) (test & 0xFF);
		}
			
		// send the UDP packet with the java.net utilities
        InetAddress address = InetAddress.getByAddress(destinationIP);
        DatagramPacket packet = new DatagramPacket( payload, payload.length, address, destinationPort );
        DatagramSocket socket = new DatagramSocket();
        socket.send(packet);

        System.out.println(byteCount + "END");
              
        socket.close();
        input.close();
	}

	public static int countOccurrences(String haystack, char needle) {
	    int count = 0;
	    for (int i = 0; i < haystack.length(); i++) {
	        if (haystack.charAt(i) == needle) { count++; }
	    }
	    return count;
	}
	
}

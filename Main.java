package etherPack0;

import java.io.IOException;
import java.net.*;

public class Pack {

	public static void main(String newDestinationIP, String newSourceIP, String newPayload) throws UnknownHostException, SocketException, IOException  {
		
		/* packet fields */
		byte[] payload = 		{ 66, 66, 69, 78, 66, 69, 78, 66, 69, 78 };
		byte[] sourceIP = 		{ (byte)198, (byte)168, (byte)1, (byte)15 };
		byte[] destinationIP =	{ (byte)198, (byte)168, (byte)1, (byte)15 };
		
		int destinationPort = 57;
		String seperator = "[.]+";
		
		/* gather inputs */
		/*Scanner input = new Scanner(System.in);
		System.out.println("Destination IP:\n");
		String newDestinationIP = input.next();
		System.out.println("Payload:\n");
		String newPayload = input.next();
		System.out.println("Source IP:\n");
		String newSourceIP = input.next();
		input.close();*/
		
		/* parse into byte arrays */
		String[] dipTokens = newDestinationIP.split(seperator);
		for (int i=0; i<dipTokens.length; i++) {
			int test = Integer.parseInt(dipTokens[i]);
			destinationIP[i] = (byte) (test & 0xFF);	// masking the int with FF allows us to use "unsigned" byte up to 255
		}
		
		char[] payloadTokens = newPayload.toCharArray();
		for (int i=0; i<payloadTokens.length; i++) {
			int test = (int) payloadTokens[i];
			payload[i] = (byte) (test & 0xFF);
		}
		
		/* TODO: look into how to edit sender IP */
		String[] sipTokens = newSourceIP.split(seperator);
		for (int i=0; i<sipTokens.length; i++) {
			int test = Integer.parseInt(sipTokens[i]);
			sourceIP[i] = (byte) (test & 0xFF);
		}
			
		/* send the UDP packet using java.net utilities */
        InetAddress address = InetAddress.getByAddress(destinationIP);
        DatagramPacket packet = new DatagramPacket(payload, payload.length, address, destinationPort);
        DatagramSocket socket = new DatagramSocket();
        socket.send(packet);

        //System.out.println("Sent");
              
        socket.close();
	}
	
}

import java.io.IOException;
import java.net.*;
import java.util.Scanner;

public class Main {

	public static void main(String[] args) throws UnknownHostException, SocketException, IOException  {
		// TODO Something for our program to start with incase user does not input anything
		byte[] buffer = {66,66,69,78,66,69,78,66,69,78};
		//byte[] SBuffer = {0,0,69,78,66,69,0,0,0,0};
		byte[] senderIP = {(byte) 198,(byte)168,(byte)1,(byte)15};
		byte [] destinationIP={(byte) 198,(byte)168,1,15};
		int[] middleMan = {0,0,0,0};
		String seperator = "[.]+";
		int destinationPort = 57;
		char divider = '.';
		int byteCount = 0;
        
		//Create Scanner
		Scanner input = new Scanner(System.in);
		
		//use Scanner to gather inputs. We are starting with IP Input first.
		System.out.println("Please enter your desired destination\n");
		System.out.println("Your current IP address is:\n");
		System.out.println(InetAddress.getLocalHost().getHostAddress());
		String newIP = input.next();
		//need to parse into a byte array
		System.out.println("Please enter your desired buffer\n");
		String newBuffer = input.next();
		System.out.println("Please enter your desired sender IP\n");
		String newSenderIP = input.next();
		
		
		//parse the inputs so they fit nicely
		//parsing newIP
		byteCount = countOccurrences(newIP, divider) +1;
		String[] tokens = newIP.split(seperator);
		//if (newIP == "0.0.0.0")
			//System.out.println("Using default IP");
			for (int i = 0; i < tokens.length; i++)
				System.out.println(tokens[i]);
			for (int i = 0; i < tokens.length; i++)
			{
				middleMan[i] = Integer.valueOf(tokens[i]);
			}
			for (int i = 0; i < middleMan.length; i++)
			{

					destinationIP[i] = (byte)middleMan[i];
			}
//------------------------------------------------------------------
		String[] buffTokens = newBuffer.split(seperator);
		for (int i = 0; i < buffTokens.length; i++)
		{
			buffer[i] = Byte.valueOf(tokens[i]);
		}
//------------------------------------------------------------------		
		//With the tools I used currently I need to look into things more to edit sender IP
		String[] sIPtokens = newSenderIP.split(seperator);
		for (int i = 0; i < sIPtokens.length; i++)
		{
			senderIP[i] = Byte.valueOf(sIPtokens[i]);
		}
//------------------------------------------------------------------	
			
		
		
		
		
		
		
		
		
			
		//Send the UDP Packet with the java.net utilities.
        InetAddress address = InetAddress.getByAddress(destinationIP);
        DatagramPacket packet = new DatagramPacket(
                buffer, buffer.length, address, destinationPort
                );
        DatagramSocket datagramSocket = new DatagramSocket();
        datagramSocket.send(packet);
        
        

        System.out.println(byteCount + "END");
        
        
        //close the socket so it does not steal all our memory
        datagramSocket.close();
        input.close();		
	}

	public static int countOccurrences(String haystack, char needle) 
	{
	    int count = 0;
	    for (int i=0; i < haystack.length(); i++)
	    {
	        if (haystack.charAt(i) == needle)
	        {
	             count++;
	        }
	    }
	    return count;
	}

}

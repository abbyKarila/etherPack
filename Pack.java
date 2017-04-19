package etherPack0;

import java.io.IOException;
import java.net.*;
import com.savarese.rocksaw.net.RawSocket;
import static com.savarese.rocksaw.net.RawSocket.PF_INET;
import static com.savarese.rocksaw.net.RawSocket.getProtocolByName;

public class Pack {

  public static class Packer {
    protected RawSocket socket;
	protected InetAddress address;
	protected byte[] data;
	protected int offset, len;
	
    protected DatagramPacket packet;
	protected byte[] payload;
	protected byte[] sourceIP;
	protected byte[] destinationIP;
	protected int destinationPort;

    protected Packer() {
	  try { address = InetAddress.getByAddress(destinationIP); }
	  catch (UnknownHostException uhe) {}
	  
      packet = new DatagramPacket(payload, payload.length, address, destinationPort);
      //packet.setIPHeaderLength(5);
      //packet.setUDPDataByteLength(56);
	  
	  //data		 = packet.getUDPPacketData();
      //offset     = packet.getIPHeaderByteLength();
      //len        = packet.getUDPPacketByteLength();
    }
	
	protected void send() {
	  socket = new RawSocket();
	  try { 
		socket.open(PF_INET, getProtocolByName("udp"));
		socket.write(address, data, offset, len);
		socket.close();
	  } catch (IOException ioe) {}
	  
	}

	public static final void main(String dIP, String sIP, String pay) throws UnknownHostException, SocketException, IOException  {
		
		/* create packet */
		final Packer p = new Pack.Packer();
		
		/* assemble packet */
		String seperator = "[.]+";
		String[] dipTokens = dIP.split(seperator);
		for (int i=0; i<dipTokens.length; i++) {
			int test = Integer.parseInt(dipTokens[i]);
			p.destinationIP[i] = (byte) (test & 0xFF);	// masking the int with FF allows us to use "unsigned" byte up to 255
		}
		
		char[] payloadTokens = pay.toCharArray();
		for (int i=0; i<payloadTokens.length; i++) {
			int test = (int) payloadTokens[i];
			p.payload[i] = (byte) (test & 0xFF);
		}
		
		String[] sipTokens = sIP.split(seperator);
		for (int i=0; i<sipTokens.length; i++) {
			int test = Integer.parseInt(sipTokens[i]);
			p.sourceIP[i] = (byte) (test & 0xFF);
		}
		
		p.destinationPort = 57;
		
		/* send packet */
		p.send();
        System.out.println("Sent");
	}
  }
  
}


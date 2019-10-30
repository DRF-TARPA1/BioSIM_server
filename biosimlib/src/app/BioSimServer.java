package app;

import java.util.ArrayList;
import java.util.List;

import quebecmrnf.weather.BioSimWeather;
import quebecmrnfutility.biosim.BioSimClient.BioSimVersion;
import quebecmrnfutility.biosim.BioSimClient.Request;
import quebecmrnfutility.biosim.ClimateVariables;
import quebecmrnfutility.biosim.PlotLocation;
import repicea.net.server.AbstractServer;
import repicea.net.server.BasicClient.ClientRequest;
import repicea.net.server.ClientThread;
import repicea.net.server.ServerConfiguration;

public class BioSimServer extends AbstractServer {

	private class BioSimClientThread extends ClientThread {

		protected BioSimClientThread(AbstractServer caller, int workerID) {
			super(caller, workerID);
		}

		@SuppressWarnings({ "unchecked", "rawtypes" })
		@Override
		protected void processRequest() throws Exception {
			List<ClimateVariables> output = new ArrayList<ClimateVariables>();
			Object req = getSocket().readObject();
			List<PlotLocation> locations;
			BioSimVersion version;
			if (req instanceof ClientRequest) {
				if ((ClientRequest) req == ClientRequest.closeConnection) {
					return;
				}
			}
			if (req instanceof Request) {
				version = ((Request) req).getBioSimVersion();
				locations = ((Request) req).getLocations();
			} else {
				version = BioSimVersion.VERSION_1971_2000;
				locations = (List) req;
			}
			for (PlotLocation location : locations) {
				ClimateVariables result = BioSimWeather.getClimateVariables(version, location.getPlotId(), location);
				output.add(result);
			}
			getSocket().writeObject(output);
		}
	}
	
	private BioSimServer(ServerConfiguration configuration) throws Exception {
		super(configuration);
	}

	@Override
	protected ClientThread createClientThread(AbstractServer server, int id) {
		return new BioSimClientThread(server, id);
	}

	
	public static void main(String[] args) {
		ServerConfiguration conf = null; 
		if (args != null && args.length >= 2) {
			try {
				int outerPort = Integer.parseInt(args[0].toString());
				int innerPort = Integer.parseInt(args[1].toString());
				conf = new ServerConfiguration(10, outerPort, innerPort);
			} catch (Exception e) {}
		}
		if (conf == null) {
			conf = new ServerConfiguration(5, 18000, 18804);
		}
		try {
			new BioSimServer(conf).startApplication();
			System.out.println("Server initialized!");
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("Unable to initialize server!");
		}
	}
	
}

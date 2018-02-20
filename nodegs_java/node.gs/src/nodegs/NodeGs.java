package nodegs;

import nodegs.service.DataCenter;
import nodegs.base.*;

public class NodeGs {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        String appPath = DataCenter.GetAppPath();
        String fileName = appPath + "\\test.js";
	if (args != null && args.length > 0)
	{
		fileName = appPath + "\\" + args[0];
	}
        DataCenter.StartService(fileName);
    }
    
}

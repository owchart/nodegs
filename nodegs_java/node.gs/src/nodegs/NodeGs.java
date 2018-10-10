package nodegs;

import nodegs.script.*;
import nodegs.service.DataCenter;
import owchart.owlib.Chart.*;
import owchart.owlib.Base.*;

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

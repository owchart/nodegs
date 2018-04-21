package nodegs;

import nodegs.script.*;
import nodegs.service.DataCenter;
import gaia.owchart.owchart_android.Chart.*;
import gaia.owchart.owchart_android.Base.*;

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

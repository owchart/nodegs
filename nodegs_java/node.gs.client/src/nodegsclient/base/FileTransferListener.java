package nodegsclient.base;

/**
 * 文件上传监听器
 * Created by joel on 7/28/17.
 */

public interface FileTransferListener {
	/**
	*文件上传监听回调方法
	*@param id 正在上传中的文件名字
	*@param totalLength 正在上传文件的总大小
	*@param currentLength 已经上传了的文件长度
	*@param errorID 错误ID,如果产生错误的话
	*@param errorMsg 错误消息,如果产生错误的话
	*/
    int OnFileTransfer(int id, long totalLength, long currentLength, int errorID, String errorMsg);
}

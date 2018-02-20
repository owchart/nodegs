/**
 *文件名字:HttpFileData.java
 * 创建时间:2017年7月20日
 */
package nodegs.base;

/**
 * 描述:<br/>
 * 
 * @author Joel
 */
public class HttpFileData {

        /// <summary>
        /// 流
        /// </summary>
        public byte[] m_buffer;

        /// <summary>
        /// 已收到长度
        /// </summary>
        public long m_recvLength;

        /// <summary>
        /// 总长度
        /// </summary>
        public long m_totalLength;
}

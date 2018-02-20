package nodegsclient.base;

import nodegsclient.base.CMessage;

/**
 * Created by Shaoli on 2016/3/17.
 */
public interface ListenerMessageCallBack
{
    /**
     监听消息

     @param message 消息
     */
    void CallListenerMessageEvent(CMessage message);
}

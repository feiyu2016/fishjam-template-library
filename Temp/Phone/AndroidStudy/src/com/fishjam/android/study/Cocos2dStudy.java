package com.fishjam.android.study;
import junit.framework.TestCase;

//http://wenku.baidu.com/view/af55f852ad02de80d4d84024.html
/***************************************************************************************************************************************
 * ע�⣺�� C/C++ ���Ե�
 * http://www.cocos2d-x.org/�� ��ȫ����ҿ�������ҵ�����Ŀ�Դ��Ϸ������ܣ�����OpenGL ES��֧��Ӳ�����١�
 *
 * Cocos2D -- 
 * Cocos2D-x -- C++��ֲ�棬֧�ֶ�ƽ̨(Windows/iOS/Android��),
 * Cocos2D-iPhone -- http://code.google.com/p/cocos2d-iphone/, ��License ����GNU LGPL v3 license����������չ��
 *    �ɾ�̬���ӻ�ֱ��ʹ��Դ�룬���ڱ�Դ����ҵ������
 * Cocos2D-JavaScript -- ����HTML5 Canvas
 *
 * �����༭��
 *   Cocos2D-Python -- ���������õ�����༭�� 
 *   Particle Designer -- 
 *   SpriteSheet Editors
 *
 * �����ļ�
 *   app.config.txt
 *   app.icf
 * 
 * ��ƽ̨���������� -- �뿪���߼���ص��ļ�����Classes��Resource�ļ��У���ƽ̨��صĴ���(������������)ͨ���ڸ���Win32��linux��Ŀ¼��
 *   Win32 -- VS
 *   Android -- ndk�ķ�ʽ�����.so
 *
 * ��
 *   ������
 *   ��Ƶ����
 **************************************************************************************************************************************/

/**************************************************************************************************************************************
 * Actions(����)--���߾����Ǹ���ʲô������ϵĶ������ƶ�(move),��ת(rotate),����(scale)��
 * Documents(�ĵ�)--���ָ�� + API�ο� + ��Ƶ��ѧ + �ܶ�������ʹ�õļ򵥲�������
 * Effects(��Ч)--��Ч��������(waves),��ת(twirl),͸��(lens)��
 * Flow control(���̿���)--������ͬ����(scenes)֮������̿���
 * Menus(�˵�)--�����ڲ��˵�
 * Sprites(����)--���ٶ�����ľ���
 * Text Rendering(�ı���Ⱦ)--֧�ֱ�ǩ��HTML��ǩ����
 * Tiled Maps(ƽ���ͼ)--֧�ְ������κ�������ƽ���ͼ
 * Transitions(ת��)--��һ�������ƶ�������һ����ͬ���ĳ���
 **************************************************************************************************************************************/

/*****************************************************************************************************
 * Director(����) ==> Scene(����) ==> Layer(��) ==> ����Ԫ��(Label,Sprite,��ͼ��)
 *   �κ�ʱ�䣬ֻ��һ��Scene����ʵ���������м���״̬
 *
 * �ܶ��඼��һ����̬�� sharedXxx ��������
 * CCApplication -- ͨ����Ҫ�̳�����(AppDelegate,Ӧ�ó������),����applicationDidFinishLaunching�г�ʼ��
 * CCDirector(����) -- ������Ϸȫ���̵ĳ����л����Ѿ�Ԥ�����˵���
 *   runWithScene() -- ����������ʱ��ʾ��һ������
 *   pushScene --
 * CCEGLView -- 
 * CCFileUtils -- 
 * CCLayer -- �㣬ͨ��ҵ�����Ӹ���̳У�����Ϊ��ͬʱ��ʾ��㣬��Ҫ͸�����͸��
 *   ϵͳ�Ѿ��ṩ�� CCMenu��CCLayerRGBA �Ȼ����
 * CCScene(����) -- ����������Ϸ�����̵Ļ��棬���¿��Է�Ϊ���¼��ࣺչʾ�ࡢѡ���ࡢ��Ϸ
 * CCSprite(����) -- ���Բ��ϱ仯��ͼƬ��������Ϸ������������Ҫ������ɻ��������
 * workflow(���̣�������?)
*****************************************************************************************************/

public class Cocos2dStudy extends TestCase{

}

package com.fishjam.android.study;
import android.content.Context;
import android.support.v4.view.ViewPager.LayoutParams;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.ImageSwitcher;
import android.widget.ImageView;
import android.widget.TabHost;
import junit.framework.TestCase;

/***************************************************************************************************************************************
 * ���(Style)
 *   �Զ���Style(ָ����ɫ����С��)
 *     1.��дstyle.xml�ļ�,����valuesĿ¼��
 *       <?xml version="1.0" encoding="utf-8"?>
 *       <resources>
 *         <style name="DavidStyleText1" [parent=����Style����]>
 *           <item name="android:textSize">18sp</item>
 *           <item name="android:textColor">#EC9237</item>
 *         </style>    ����ʹ�� <style></style> ������,
 *       </resources>
 *     2.main.xml �� <TextView style="@style/DavidStyleText1" ...>
 *     
 * ����(Theme)
 * 
 * view  -- ����UI�඼������View��ViewGroup������Ļ���֮�ϣ��������(Composite)���ģʽ
 *   View --�����ؼ�,  �������Ϊ Widget
 *     Button( setOnClickListener(new Button.OnClickListener(){public void onClick(View v) {...}}); )
 *     EditText
 *     TextView
 *     RelativeLayout
 *     lists,grids,text boxes,buttons,
 *   ViewGroup -- ���������������ΪLayout
 *   
 * �Ի�
 *   ��View������������ onDraw(Canvas canvas) ��������������ʹ�� Canvas �� Paint���л��ơ�
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * View
 *   AutoCompleteTextView -- �Զ�����ı���ͨ��������Ҫ��ʾ��Դ��������(setAdapter)��ʵ��
 *   BaseAdapter -- 
 *   Button
 *     text
 *   EditText -- �༭��������
 *     textSize -- 18sp
 *     password -- true
 *     numeric -- decimal
 *   CheckBox -- ��ѡ��ť
 *   Gallery(ͼ��) + ImageSwitcher -- ��ˮƽ������ʾ�����ݣ�һ���������ͼƬ����ѡ�е���λ���м䣬�ҿ�����Ӧ�¼���ʾ��Ϣ��
 *      ͨ��ʹ�� Gallery ��ʾ����ͼ���ڵ��ʱͨ��ImageSwitcher::setImageResource�л�����ͼ
 *      ImageSwitcher::setOutAnimation(AnimationUtils.loadAnimation(this, android.R.anim.fade_out)); // ���ö���Ч��
 *   GridView -- �������еķ�ʽ��ʾ���ݣ�һ���ʺ���ʾͼƬ)
 *   ImageButton
 *     src -- "@drawable/iconempty"
 *     setImageResource(R.drawable.iconempty);
 *   ImageView -- ��ʾͼƬ
 *     setImageDrawable(getResources().getDrawable(R.drawable.right));   //������ʾ��ͼƬ, ��ͨ�� src ��������
 *     setImageResource(xxx);
 *   ListView/ListActivity -- �б���ͼ���Դ�ֱ�б��ķ�ʽ�г���Ҫ��ʾ���б���(����ϵ��������ϵͳ�������)
 *     �ؼ��㣺����Adapter
 *   MapView -- ��ʾGoogle��ͼ
 *   ProgressBar -- ���������кܶ���:
 *     1.ProgressDialog(�Ի��������)�� ����Activity��onCreateDialog�������������д��������������أ�����showDialog������ʾ;
 *     2.ProgressBarIndeterminate(������������)������ Activity.requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS) �������ô����н�����������;
 *       ���� Activity.setProgressBarIndeterminateVisibility ��ʾ������
 *     3.ProgressBar(ˮƽ������) -- �����ļ�������ProgressBar����ȡ����������� incrementProgressBy �ȷ������ý���
 *   RadioGroup(����һ��RadioButton)
 *     checkedButton -- "@+id/sex1"
 *     orientation -- vertical,horizontal
 *   RadioButton
 *   Spinner -- �����б�
 *   TabHost / TabActivity -- ѡ�
 *   TextView -- �ı���ͼ����ʾ�ַ���
 *     text -- @string/str_id
 *     textColor -- @drawable/darkgray
 *     gravity="center_vertical|center_horizontal", bottom
 *     autoLink -- all(������ʾ��ַ���� http://)
 *   ToggleButton -- ���ذ�ť
 *   WebView -- ����������ؼ�����ֱ�Ӽ�����ҳ��Ϊ��Ӧ�����ӹ��ܣ����� setWebViewClient ���������Զ���� WebViewClient ����ʵ��
 *     getSettings().setJavaScriptEnabled(true) -- ��������
 *     loadUrl -- ����ָ����URL��ַ��ҳ
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * Layout -- Android ͨ�� LayoutInflater �ཫ XML ��ʽ�Ĳ����ļ��е��������Ϊ���ӻ�����ͼ�����
 *   �����ļ��е� <requestFocus/> �����ʲô��˼? 
 *   AbsoluteLayout -- ����λ�ö�λ�����ͼ����ԣ�ά���ɱ��� ( TODO: ������ )
 *   FrameLayout -- ֡���֣��������Ļ�����Ͻǿ�ʼ���֣�������������򣬺�����������ǰ��������
 *   LinearLayout -- ���Ͳ��֣����մ�ֱ����ˮƽ���򲼾����
 *     gravity(���뷽ʽ) -- top, bottom, left, right, center_vertical
 *     orientation(����) -- vertical,horizontal
 *     layout_width -- fill_parent,wrap_content(��������ָ���߿�),320px,80dip
 *     layout_height -- fill_parent,wrap_content
 *     layout_alignParentRight -- true
 *     layout_alignParentTop -- true
 *     layout_alignParentLeft -- true
 *     layout_marginRight -- 80dip, 
 *     layout_below -- "@+id/ExitButton", 
 *     layout_alignTop -- "@+id/sizebutton"
 *     layout_toLeftOf -- "@+id/sizebutton"
 *     layout_x, layout_y -- 30px
 *     layout_centerHorizontal -- true
 *     id -- "@+id/ResultText"
 *    background -- "@drawable/testpic",
 *   RelativeLayout -- ��Բ��֣������������Ĳ��ַ�ʽ(�������������ҵ�)�����������Զ���Ӧ���������ͼ����Ρ�
 *     layout_below, layout_toLeftOf
 *   TableLayout  -- ���񲼾֣��������з�ʽ�������( ���� > TableRow > Ԫ�� �ķ�ʽ)
 *     stretchColumns -- "����"
 *   TableRow
 *
***************************************************************************************************************************************/

/**************************************************************************************************************************************
 * �Ի���(Dialog)
 *   AlertDialog(����Ի���)
 *     ʾ��: 
 *       AlertDialog.Builder dlgBuilder = new AlertDialog.Builder(this);  //����Builder��Ȼ������������ø��ֲ���
 *       dlgBuilder.setMessage(msg).setCancelable(false).setPositiveButton("ȷ��", new DialogInterface.OnClickListener() { ...onClick(...){finish();} }); //����builder
 *       AlertDialog alert = dlgBuilder.create();  //�����Ի���ʵ��
 *       alert.show(); //��ʾ
 *  DatePickerDialog/TimePickerDialog(����/ʱ�� ѡ��Ի���) --  
 *    1.���� Activity.onCreateDialog(int id) { switch(id) {...}  } ���������Ի���;
 *    2.�� OnDateSetListener �� OnTimeSetListener �Ķ�Ӧ�¼���������Ӧ���ں�ʱ�������;
 *    3.���� Activity.showDialog ��ʾ�Ի���
 *  
 * Toast -- ��ʾ��Ϣ(û�н������ܣ�ֻ����ʾ), ��̬��makeText������ʾ�ı���ʱ��
 *    Toast.makeText(getApplicationContext(), "��ʾ��Ϣ", Toast.LENGTH_LONG).show;
**************************************************************************************************************************************/

/***************************************************************************************************************************************
 * Activity  :  ��ʾ�û������е�һ����Ļ����ʾ�ɼ���Views�ؼ���ɵ��û��ӿڣ������¼�������Ӧ��������xml�ļ��ж�������Ҫ��Views���ڴ����ж��壬
 *     Context <- ContextWrapper <- ContextThemeWrapper <- Activity
 *   ÿ��Activity��״̬��������Activityջ�е�λ�������������е�ǰ�������е�Activity����ѭ����ȳ���ԭ��
 *   ��һ���µ�Activity��������ǰ��Activity��������ջ�Ķ���������û�ʹ��Back��ť������ǰ̨Activity���رգ���һ��Activity�����������������ջ�Ķ�����
 *   Activity״̬ -- ״̬���ɾ��в�ȷ���Բ�����OPhone Runtime��ȫ����
 *     Active״̬����ʱ��Activity����ջ�������ǿɼ��ģ��н���ģ��ܹ������û�����ǰ��Activity��
 *     Paused״̬��Activity�ǿɼ��ģ���û�н��㡣���磬�����һ��͸�����ȫ��Ļ�ϵ�Activity�����Activity���档
 *     Stopped״̬����ȫ����ס�����ɼ���ʱ���������������ڴ��б��ֵ�ǰ������״̬�ͳ�Ա��Ϣ�������Ǳ����ն������Ҫ��ѡ��
 *     Inactive״̬��Activity��ɱ���Ժ���߱�������ǰ���ѱ���Activity��ջ���Ƴ�����Ҫ���������ſ�����ʾ��ʹ�á�
 *   ���Activity��״̬�仯
 *     onCreate -- ��Activity�������ڿ�ʼʱ�����ã���ʼ���û����棬��������������������ݿؼ���������������߳� -- ͨ����ҪfindViewById����Ա�����Ϳؼ�����������
 *     onRestore -- onCreate��ɺ󱻵��ã������ظ�UI״̬
 *     onRestart -- ��activity��ֹͣ״̬��������ʱ����(onStop->onRestart->onStart)
 *     onStart -- ��activity���û������ɼ���ʱ����ã���ע������û�����Intent������
 *     onResume -- ��activity��Ҫ���û�����ʱ���ô˷�������ʱactivity��activityջ��ջ�����û������Ѿ����Դ��ݸ��� 
 *     onSave -- Activity�����Ƴ�ջ������UI״̬ʱ���ô˷���
 *     onPause -- ��ϵͳҪ����һ��������activityʱ���ã���������������ύ��Щ�־����ݵĸı䡢ֹͣ������������ռ�� CPU��Դ�Ķ�����
 *     onStop -- ������һ��activity�ָ����ڸ�ס��activity,��������û����ٿɼ�ʱ���á�һ����activity����������activity���л���ǰ������ǰactivity������ʱ���ᷢ�����ֳ�������ע�������û�����Intent������
 *     onDestroy -- ��activity������ǰ�����õ����һ���������п�����ĳЩ����£�һ��Activity����ֹʱ��������onDestroy������
 *   ��������
 *     findViewById
 *     setContentView -- �ɽ�Activity��Views����һ����ʵ���û������Ĺ���
 *     startActivity -- ��һ����Ļ��������һ����Ļ����Activity����������װ��Intent��
 **************************************************************************************************************************************/

/**************************************************************************************************************************************
 * ��ť����
 *   1. implements OnClickListener
 *      Button ResolutionTest = (Button)findViewById(R.id.ResolutionTest); 
 *      ResolutionTest.setOnClickListener(this);  //������ onClick�бȽ� getId���ܴ��������ť�ĵ�����
 *      public void onClick(View arg0) { 
 *          if (arg0.getId() == R.id.ResolutionTest)  { startTest(); } 
 *      } 
 *   2. ʹ�ó�Ա����
 *       ResolutionTest.setOnClickListener(m_myButtonClick);
 *       Button.OnClickListener m_myButtonClick =  new Button.OnClickListener() {...}
 *   3. ʹ�������¼�
 *       ResolutionTest.setOnClickListener(new Button.OnClickListener() {
 *           //@Override -- �Ƿ���Ҫ����
 *           public void onClick(View v) {
 *              startTest();  //���ñȽ�getId,ÿ��Button���������Լ���OnClick
 *           }
 *       });
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 1. ����TitleBar �� Status Bar -- TODO����Ҫ��setContentView(R.layout.start_test)֮ǰ���ã���������force close ����
 *    �ڵ�ǰActivity��JavaԴ�ļ�����android.view.Window��android.view.WindowManager������oncreate()�����е���:
 *    requestWindowFeature(Window.FEATURE_NO_TITLE);   		//Hide title bar;
 *    getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);  //Hide status bar;
 *    //getWindow().setBackgroundDrawableResource(R.color.red_bg); //���ñ���ɫ
 * 2. ͨ����Դ�ж����id�ҵ���Ӧ�Ķ��� -- findViewById
 *    TextView ResultText = (TextView)findViewById(R.id.ResultText);
 * 3. ��ʾͼƬ
 *    
**************************************************************************************************************************************/

public class UITester extends TestCase {
	public UITester()	{
		
		
	}
	protected void setUp() throws Exception {
		super.setUp();

	}

	protected void tearDown() throws Exception {
		super.tearDown();
	}
	
	//�����л� -- ��ʲô����?��׼Ӧ����ʹ�õڶ��ַ�������Ȼ�鷳�����������ͣ�
	//  1.�л�layout -- ���岻ͬ��layout xml�ļ�,ͨ�� setContentView���� �����л�; ����ͬһ��Activity�У���Ա�������䡣
	//  2.�л�Activity -- ���岻ͬ��layout xml��ʵ�ֲ�ͬ��Activity����(��ʾ��Ӧ��layout),ͨ�� Intent�� �����л���
	//  ע����Ҫ��AndroidManifest.xml�ж����µ�activity����������ô���ԣ������޷����롣
	public void TestSwitchUI() {
		/*************************************************************************************************
		 * Intent intent = new Intent();// newһ��Intent����
		 * intent.setClass(EX03_09.this, EX03_09_1.class); //ָ��Ҫ������class
		 * startActivity(intent);// ����һ���µ�Activity -- ���Ʒ�ģ̬��
		 * //startActivityForResult(intent,0); // ������ActivityҪ����ֵ������,
		 * �����µ�Activity���ȴ�������� // ��ͨ������ onActivityResult �����жϷ���ֵ(resultCode ==
		 * RESULT_OK), ����ͨ�� data.getExtras ��ȡ���ݣ� ���в�ι�ϵ // ��Activity��ͨ�����·�ʽ���ؽ��:
		 * // EX03_09_1.this.setResult(RESULT_OK, intent); //
		 * ����result����һ��activity // EX03_09_1.this.finish();// �������activity
		 * EX03_09.this.finish(); // �ر�ԭ����Activity(���ݾ������)
		 **************************************************************************************************/
	}
	
	public void TabHostTester(){
		/********************************************************************************************
		 * ����1:
		 *   1. �ڲ����ļ���ʹ�� FrameLayout �г�Tab�����Tab�е��������
		 *   2. Activity Ҫ�̳� TabActivity;
		 *   3.���� TabActivity::getTabHost ��� TabHost ����Ȼ�󴴽� Tab ѡ��ҳ
		 * ����2:
		 *   1.ʵ�ֽӿ� TabHost.TabContentFactory �� createTabContent ������ָ��
		 *     public class MyActivity extends TabActivity implements TabHost.TabContentFactory { 
		 *          onCreate ��:  getTabHost().addTab(getTabHost().newTabSpec("all").setContent(this);
		 *          public View createTabContent(String tag) { ...; if(tag.equals("all")) {  ����allҳ���Ӧ������  }  } 
		 ********************************************************************************************/
		
		/*******************************************************************************************
		//���벿��
			TabHost tabHost = getTabHost();
			LayoutInflater.from(this).infalte(R.layout.main, tabHost.getTabContentView(), true);
			tabHost.addTab(tabHost.newTabSpec("all").setIndicator("����ͨ����¼").setContent(R.id.TextViewAll));
			tabHost.addTab(tabHost.newTabSpec("ok").setIndicator("�ѽ�����").setContent(R.id.TextViewReceived));
			tabHost.addTab(tabHost.newTabSpec("cancel").setIndicator("δ������").setContent(R.id.TextViewUnreceived));

		 //���ֲ���
		   <FrameLayout ....>
		   	  <TabHost>
		   	    <TextView android:id="@+id/TextViewAll" android:layout_width="wrap_content" android.text="����ͨ����¼">
		   	    </TextView>
		   	  </TabHost>
		   </FrameLayout>
		 *******************************************************************************************/
	}
	

	public void GridViewDisplayImageTester(){
		//ʹ�� BaseAdapter �࣬��д���еķ���( �� getView ������ͼƬ��ʾ��ʽ )
		/********************************************************************************************
		//�Զ���� BaseAdapter �࣬������Ҫ���ز�ͼƬ��������Ϣ
			class MyAdapter extends BaseAdapter{
				private Integer [] imgs = {
						R.drawable.ic_launcher
				};
				Context context;
				MyAdapter(Context context){
					this.context = context;
				}
				@Override
				public View getView(int position, View convertView, ViewGroup parent) {
					ImageView imageView;
					if(convertView == null){
						imageView = new ImageView(context);
						imageView.setLayoutParams(new GridView.LayoutParams(45, 45));	//����ImageView���󲼾�
						imageView.setAdjustViewBounds(false); 		//���ñ߽����
						imageView.setPadding(8,  8,  8,  8);				//���ü��
					}
					else{
						imageView = (ImageView)convertView;
					}
					imageView.setImageResource(imgs[position]);	//Ϊ ImageView ����ͼƬ��Դ
					return imageView;
				}
			}
		//Activity �����У�ʹ���Զ���� Adapter �� GridView ����ʾͼƬ
			GridView gridView = (GridView)findViewById(R.id.xxx);
			gridView.setNumColumns(4); 	//��������
			gridView.setAdapter(new MyAdapter(this));
		*********************************************************************************************/
	}
}

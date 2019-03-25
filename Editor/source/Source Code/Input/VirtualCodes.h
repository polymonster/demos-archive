#define VK_LBUTTON			0x01
#define VK_RBUTTON			0x02
#define VK_CANCEL			0x03
#define VK_MBUTTON			0x04
 
#define VK_BACK				0x08	 
#define VK_TAB				0x09 
	
#define VK_CLEAR			0x0C 
#define VK_RETURN			0x0D 

#define VK_SHIFT			0x10  
#define VK_CONTROL			0x11 
#define VK_MENU				0x12
#define VK_PAUSE			0x13  
#define VK_CAPITAL			0x14

#define VK_ESCAPE			0x1B
#define VK_SPACE			0x20
#define VK_PRIOR			0x21   
#define VK_NEXT				0x22 
#define VK_END				0x23 
#define VK_HOME				0x24 
#define VK_LEFT				0x25 
#define VK_UP				0x26
#define VK_RIGHT			0x27 
#define VK_DOWN				0x28 
#define VK_SELECT			0x29

#define VK_EXECUTE			0x2B 
#define VK_SNAPSHOT			0x2C 
#define VK_INSERT			0x2D 
#define VK_DELETE			0x2E
#define VK_HELP				0x2F		 
#define VK_0				0x30	  
#define VK_1				0x31					
#define VK_2				0x32					   
#define VK_3				0x33				   
#define VK_4				0x34					   
#define VK_5				0x35					   
#define VK_6				0x36					   
#define VK_7				0x37					   
#define VK_8				0x38					   
#define VK_9				0x39					   
										
#define VK_A				0x41					   
#define VK_B				0x42					   
#define VK_C				0x43					   
#define VK_D				0x44					   
#define VK_E				0x45					   
#define VK_F				0x46					   
#define VK_G				0x47					   
#define VK_H				0x48					   
#define VK_I				0x49					   
#define VK_J				0x4A					   
#define VK_K				0x4B					   
#define VK_L				0x4C					   
#define VK_M				0x4D					   
#define VK_N				0x4E					  
#define VK_O				0x4F					   
#define VK_P				0x50					   
#define VK_Q				0x51					   
#define VK_R				0x52					   
#define VK_S				0x53					   
#define VK_T				0x54					   
#define VK_U				0x55					   
#define VK_V				0x56					   
#define VK_W				0x57					   
#define VK_X				0x58					   
#define VK_Y				0x59					   
#define VK_Z				0x5A					   
#define VK_NUMPAD0			0x60			  
#define VK_NUMPAD1			0x61			  
#define VK_NUMPAD2			0x62			  
#define VK_NUMPAD3			0x63			  
#define VK_NUMPAD4			0x64			  
#define VK_NUMPAD5			0x65			 
#define VK_NUMPAD6			0x66			 
#define VK_NUMPAD7			0x67			  
#define VK_NUMPAD8			0x68			  
#define VK_NUMPAD9			0x69			  
#define VK_MULTIPLY			0x6A				 
#define VK_ADD				0x6B				  
#define VK_SEPARATOR		0x6C				  
#define VK_SUBTRACT			0x6D				  
#define VK_DECIMAL			0x6E				  
#define VK_DIVIDE			0x6F				  
#define VK_F1				0x70				 
#define VK_F2				0x71				  
#define VK_F3				0x72				  
#define VK_F4				0x73				  
#define VK_F5				0x74				  
#define VK_F6				0x75				  
#define VK_F7				0x76				  
#define VK_F8				0x77				  
#define VK_F9				0x78				  
#define VK_F10				0x79				  
#define VK_F11				0x7A				  
#define VK_F12				0x7B				  
	 	   
#define VK_NUMLOCK			0x90
#define VK_SCROLL			0x91

// And a list of the important virtual codes just for you
// "Missing codes" are most likely OEM (original equipment manufacture) specific so check
// out the website of the keyboard in question or MSDN for a full virtual code list

/*	
	Virtual Code	Hex Value			"Key"
	============	=========			=====

	 VK_LBUTTON			01			left mouse button  
	 VK_RBUTTON			02			right mouse button  
	 VK_CANCEL			03			control-break processing  
	 VK_MBUTTON			04			middle mouse button (three-button mouse)  
 
	 VK_BACK			08				backspace  
	 VK_TAB				09				tab  
	
	 VK_CLEAR			0C				clear  
	 VK_RETURN			0D				enter  

	 VK_SHIFT			10				shift  
	 VK_CONTROL			11				ctrl   
	 VK_MENU			12				alt 
	 VK_PAUSE			13				pause   
	 VK_CAPITAL			14				caps lock  

	 VK_ESCAPE			1B				esc  
	 VK_SPACE			20				spacebar  
	 VK_PRIOR			21				page up   
	 VK_NEXT			22				page down  
	 VK_END				23				end key  
	 VK_HOME			24				home key  
	 VK_LEFT			25				left arrow  
	 VK_UP				26				up arrow  
	 VK_RIGHT			27				right arrow  
	 VK_DOWN			28				down arrow  
	 VK_SELECT			29				select key  

	 VK_EXECUTE			2B				execute key  
	 VK_SNAPSHOT		2C				print screen key  
	 VK_INSERT			2D				insert key  
	 VK_DELETE			2E				delete key  
	 VK_HELP			2F				help key  
	 VK_0				30					0   
	 VK_1				31					1   
	 VK_2				32					2   
	 VK_3				33					3   
	 VK_4				34					4   
	 VK_5				35					5   
	 VK_6				36					6   
	 VK_7				37					7   
	 VK_8				38					8   
	 VK_9				39					9   
											
	 VK_A				41					a   
	 VK_B				42					b   
	 VK_C				43					c   
	 VK_D				44					d   
	 VK_E				45					e   
	 VK_F				46					f   
	 VK_G				47					g   
	 VK_H				48					h   
	 VK_I				49					i   
	 VK_J				4A					j   
	 VK_K				4B					k   
	 VK_L				4C					l   
	 VK_M				4D					m   
	 VK_N				4E					n   
	 VK_O				4F					o   
	 VK_P				50					p   
	 VK_Q				51					q   
	 VK_R				52					r   
	 VK_S				53					s   
	 VK_T				54					t   
	 VK_U				55					u   
	 VK_V				56					v   
	 VK_W				57					w   
	 VK_X				58					x   
	 VK_Y				59					y   
	 VK_Z				5A					z   
 	 VK_NUMPAD0			60			Numeric keypad 0 key  
	 VK_NUMPAD1			61			Numeric keypad 1 key  
	 VK_NUMPAD2			62			Numeric keypad 2 key  
	 VK_NUMPAD3			63			Numeric keypad 3 key  
	 VK_NUMPAD4			64			Numeric keypad 4 key  
	 VK_NUMPAD5			65			Numeric keypad 5 key  
	 VK_NUMPAD6			66			Numeric keypad 6 key  
	 VK_NUMPAD7			67			Numeric keypad 7 key  
	 VK_NUMPAD8			68			Numeric keypad 8 key  
	 VK_NUMPAD9			69			Numeric keypad 9 key  
	 VK_MULTIPLY		6A				Multiply key  
	 VK_ADD				6B				Add key  
	 VK_SEPARATOR		6C				Separator key  
	 VK_SUBTRACT		6D				Subtract key  
	 VK_DECIMAL			6E				Decimal key  
	 VK_DIVIDE			6F				Divide key  
	 VK_F1				70				f1 key  
	 VK_F2				71				f2 key  
	 VK_F3				72				f3 key  
	 VK_F4				73				f4 key  
	 VK_F5				74				f5 key  
	 VK_F6				75				f6 key  
	 VK_F7				76				f7 key  
	 VK_F8				77				f8 key  
	 VK_F9				78				f9 key  
	 VK_F10				79				f10 key  
	 VK_F11				7A				f11 key  
	 VK_F12				7B				f12 key  
	 	   
	 VK_NUMLOCK			90			num lock key  
	 VK_SCROLL			91			scroll lock key  
*/
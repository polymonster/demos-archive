#ifndef editor_states_h__
#define editor_states_h__




class EditorStates
{
public:

	EditorStates();

	int m_main_mode;

	enum
	{
		VIEWER = -1,
		OBJECT,
		LIGHTING

	}e_main_modes;
};

extern EditorStates g_editor_states;

#endif // editor_states_h__
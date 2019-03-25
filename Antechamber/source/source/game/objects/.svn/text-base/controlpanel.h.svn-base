#ifndef _CONTROL_PANEL_H
#define _CONTROL_PANEL_H

#include  "quad.h"
#include  "soundManager.h"

typedef enum
{
	GATE_CONTROL = 0,
	INTERCOM,
	SAVE_POS,

}e_cp_types;

class ControlPanel : public Quad
{
	public:

		ControlPanel( void );
		ControlPanel(vector2 position, int gate_id, char *name_p, int type);
		~ControlPanel( void );

		virtual void		execute					( void );
		virtual void		render					( void );
		bool				activate				( int *gate_id );

		int					get_type				( void )		{ return m_type; }

		bool				play_dialogue			();
		void				assign_sound			(char *file);

		void				stop					();

	private:

		int m_gate_id;
		int m_type;

		char m_sound_file[64];
		char m_text_file[64];
		
		char m_dialogue_lines[10][128];
		int	 m_dlg_lines;

		bool m_intercom;
		bool m_playing;
		float m_dlg_len;
		float m_dlg_start;

		tga_texture_s m_tex2;

		FSOUND_STREAM *stream;
};

#endif
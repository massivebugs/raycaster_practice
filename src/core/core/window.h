#ifndef ZJVL_CORE_WINDOW_H
#define ZJVL_CORE_WINDOW_H
#include "all.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "core/event/subject.h"
#include "input.h"

namespace ZJVL
{
	namespace Core
	{
		class Window : public Subject
		{
		public:
			Window();
			Window(const char *name, int w, int h);
			Input input_manager;

			bool init();
			void update();
			void cleanup();

			bool draw(void *buffer, int row_bytes);
			bool flash_image(const char *img_path, std::uint32_t ms);

		private:
			const char *m_name;
			int m_width, m_height;

			SDL_Window *m_window;
			SDL_Renderer *m_renderer;
			SDL_Texture *m_texture;
			SDL_Event m_event;
		};
	}
}
#endif

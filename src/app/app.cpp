#include <iostream>
#include <math.h>
#include <memory>
#include "app.h"
#include "core/scene/scene.h"
#include "core/render/framebuffer.h"

namespace ZJVL
{
	App::App(const char *app_name, int win_w, int win_h) : m_window(app_name, win_w, win_h), m_renderer(win_w, win_h)
	{
		m_app_name = app_name;
		m_framecount = 0;
		m_running = true;
	}

	// Begins game loop execution
	int App::run()
	{
		return on_execute();
	}

	// This is the main execution loop,
	// where all of the game loop functionalities
	// are tied together.
	int App::on_execute()
	{
		// Handle initialization failures
		if (on_init() == false)
		{
			return -1;
		}

		// Set up the game here
		Framework::Timer fps_timer;

		std::vector<Core::Entity> entities = std::vector<Core::Entity>{{3.523, 3.812, NULL, NULL, 2}, {1.834, 8.765, NULL, NULL, 0}, {5.323, 5.365, NULL, NULL, 1}, {4.123, 10.265, NULL, NULL, 2}};
		Core::Entity player{3.456, 2.345, 1.523, M_PI / 3.};
		Core::Map map;
		Core::Scene scene{map, player, entities};
		scene.load_splash(Core::Splash{"assets/splash_zjvl.png", 3000});
		current_scene = scene;

		while (m_running)
		{
			// if (!current_scene.is_activated())
			// {
			// 	// Loop over and display the splash images
			// 	for (Core::Splash &splash_image : current_scene.splash_images)
			// 	{
			// 		m_window.flash_image(splash_image.img_path, splash_image.display_ms);
			// 	};
			// }

			// An example of how the Timer class can be used as an FPS counter.
			// Use .reset() and .get_duration()
			// to retrieve how many miliseconds has passed since the last .reset()
			if (fps_timer.get_duration() >= 1000)
			{
				std::cout << "FPS: " << m_framecount << std::endl;
				m_framecount = 0;
				fps_timer.reset();
			}

			on_event();
			on_update();
			on_render();
			m_framecount++;
		}

		on_cleanup();
		return 0;
	}

	void App::on_event()
	{
		m_window.poll_event();
	}

	void App::on_notify(Core::Event &e)
	{
		switch (e.get_type())
		{
		case Core::EventType::WINDOW_QUIT:
			m_running = false;
			break;
		case Core::EventType::KEYDOWN:
			switch (static_cast<Core::KeyDownEvent&>(e).get_key())
			{
			case Core::Key::ENTER:
				break;
			case Core::Key::ESC:
				break;
			case Core::Key::W:
				current_scene.player.x += cos(current_scene.player.angle) * 0.1;
				current_scene.player.y += sin(current_scene.player.angle) * 0.1;
				break;
			case Core::Key::A:
				current_scene.player.y += 0.1;
				break;
			case Core::Key::S:
				current_scene.player.x -= cos(current_scene.player.angle) * 0.1;
				current_scene.player.y -= sin(current_scene.player.angle) * 0.1;
				break;
			case Core::Key::D:
				current_scene.player.x -= 0.1;
				break;
			}
			break;
		}
	}


	// This is where any post processing gameplay updates may happen.
	// Note that this is called after the event handlers have been called,
	// and is the final step before updates to do any kind of processing.
	void App::on_update()
	{
		m_window.get_mouse();
		current_scene.player.angle += m_window.mouse_x * M_PI / 360;
	}

	bool App::on_init()
	{
		m_renderer.init();
		m_window_subject = m_window.add_observer(this);
		return m_window.init();
	}

	void App::on_render()
	{
		m_renderer.set_scene(current_scene);
		Core::FrameBuffer *framebuffer = m_renderer.render();
		m_window.draw(static_cast<void *>(framebuffer->img.data()), framebuffer->w * 4);
	}

	void App::on_cleanup()
	{
		m_renderer.cleanup();
		m_window.remove_observer(m_window_subject);
		m_window.cleanup();
	}
}
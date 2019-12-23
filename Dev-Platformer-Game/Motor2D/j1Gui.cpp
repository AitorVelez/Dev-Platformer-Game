#include "j1App.h"
#include "j1Textures.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Module.h"
#include "p2Log.h"
#include "p2List.h"
#include "j1Input.h"
#include "UIObject.h"


j1Gui::j1Gui() : j1Module() {

	name.create("gui");

}


j1Gui::~j1Gui() {}

bool j1Gui::Awake(pugi::xml_node& conf) {

	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

bool j1Gui::Start() {

	atlas = App->tex->Load(atlas_file_name.GetString());
	return true;
}

bool j1Gui::PreUpdate() {


	return true;
}

bool j1Gui::Update(float dt) {

	for (int i = 0; i < UIObjects.count(); ++i) {

		if (UIObjects.At(i) != nullptr) {

			UIObjects.At(i)->data->Draw();
			UIObjects.At(i)->data->Update();

		}
	}

	for (int i = 0; i < HUDObjects.count(); i++)
	{
		if (HUDObjects.At(i) != nullptr)
		{
			HUDObjects.At(i)->data->Draw();
			HUDObjects.At(i)->data->Update();
		}
	}

	return true;
}

bool j1Gui::PostUpdate() {



	return true;
}

bool j1Gui::CleanUp() {

	p2List_item<UIObject*>* item = UIObjects.start;
	while (item != nullptr)
	{
		delete item->data;
		UIObjects.del(item);
		item = item->next;
	}
	return true;
}

bool j1Gui::HUDCleanUp()
{
	p2List_item<UIObject*>* item = HUDObjects.start;
	while (item != nullptr)
	{
		delete item->data;
		HUDObjects.del(item);
		item = item->next;
	}
	return true;
}






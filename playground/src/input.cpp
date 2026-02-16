// input.cpp

#include "input.hpp"
#include "raymath.h"
#include <cassert>
#include <cstdio>

float KeyBinding::value() const
{
   return m_value;
}

bool KeyBinding::down() const
{
   return m_down;
}

bool KeyBinding::pressed() const
{
   return m_pressed;
}

bool KeyBinding::released() const
{
   return m_released;
}

void KeyBinding::clear()
{
   m_count = 0;
}

KeyBinding& KeyBinding::add(unsigned int scancode)
{
   assert(m_count < _countof(m_keys));
   m_keys[m_count++] = scancode;
   return *this;
}

float AxisBinding::value() const
{
   return m_positive.value() - m_negative.value();

#if 0 // alternative approach
   float pos = m_positive.value();
   float neg = m_negative.value();
   
   if (pos <= 0.0f && neg <= 0.0f) {
      return 0.0f;
   }
   if (pos > 0.0f && neg <= 0.0f) {
      return pos;
   }
   if (neg > 0.0f && pos <= 0.0f) {
      return -neg;
   }

   return 0.0f;
#endif
}

bool AxisBinding::down() const
{
   return m_positive.down() || m_negative.down();
}

bool AxisBinding::pressed() const
{
   return m_positive.pressed() || m_negative.pressed();
}

bool AxisBinding::released() const
{
   return m_positive.released() || m_negative.released();
}

void AxisBinding::clear()
{
   m_positive.clear();
   m_negative.clear();
}

AxisBinding& AxisBinding::add(unsigned int negative, unsigned int positive)
{
   m_negative.add(negative);
   m_positive.add(positive);
   return *this;
}

Vector2 DirectionBinding::value() const
{
   Vector2 result{ m_x.value(), m_y.value() };
   return Vector2Normalize(result);
}

bool DirectionBinding::down() const
{
   return m_x.down() || m_y.down();
}

bool DirectionBinding::pressed() const
{
   return m_x.pressed() || m_y.pressed();
}

bool DirectionBinding::released() const
{
   return m_x.released() || m_y.released();
}

void DirectionBinding::clear()
{
   m_x.clear();
   m_y.clear();
}

DirectionBinding& DirectionBinding::add(unsigned int left, unsigned int right, unsigned int up, unsigned int down)
{
   m_x.add(left, right);
   m_y.add(up, down);
   return *this;
}

bool InputContext::save(const std::string_view path) const
{
   FILE* file = nullptr;
   errno_t error = fopen_s(&file, path.data(), "wb");
   if (file == nullptr) {
      return false;
   }

   /*
   key  "name" <number-of-keys> [<keycode>, ...]
   axis "name" <number-of-keys> [<keycode>, ...] <number-of-keys> [<keycode>, ...]
   dir  "name" <number-of-keys> [<keycode>, ...] <number-of-keys> [<keycode>, ...] <number-of-keys> [<keycode>, ...] <number-of-keys> [<keycode>, ...]
   */

   auto save_binding_header = [](FILE* file, const std::string_view key, const std::string_view value) {
      fprintf(file, "%s \"%s\"", key.data(), value.data());
   };

   auto save_key_bindings = [](FILE* file, const KeyBinding& binding) {
      fprintf(file, " %d", binding.m_count);
      for (unsigned int index = 0; index < binding.m_count; index++) {
         fprintf(file, " %d", binding.m_keys[index]);
      }
   };

   auto save_endline = [](FILE* file) {
      fprintf(file, "\r\n");
   };

   save_binding_header(file, "version", "1");
   save_endline(file);

   // note: save all key mappings
   for (const auto& action : m_key_actions) {
      const auto& name = action.name;
      const auto& data = action.data;
      save_binding_header(file, "key", name);
      save_key_bindings(file, data);
      save_endline(file);
   }

   for (const auto& action : m_axis_actions) {
      const auto& name = action.name;
      const auto& data = action.data;
      save_binding_header(file, "axis", name);
      save_key_bindings(file, data.m_negative);
      save_key_bindings(file, data.m_positive);
      save_endline(file);
   }

   for (const auto& action : m_dir_actions) {
      const auto& name = action.name;
      const auto& data = action.data;
      save_binding_header(file, "dir", name);
      save_key_bindings(file, data.m_x.m_negative);
      save_key_bindings(file, data.m_x.m_positive);
      save_key_bindings(file, data.m_y.m_negative);
      save_key_bindings(file, data.m_y.m_positive);
      save_endline(file);
   }

   fclose(file);

   return true;
}

void InputContext::update()
{
   auto key_update = [](KeyBinding& binding) {
      binding.m_value = 0.0f;
      binding.m_down = false;
      binding.m_pressed = false;
      binding.m_released = false;

      for (unsigned int index = 0; index < binding.m_count; index++) {
         unsigned int key = binding.m_keys[index];
         if (IsKeyDown(key)) {
            binding.m_value = 1.0f;
            binding.m_down = true;
         }

         if (IsKeyPressed(key)) {
            binding.m_value = 1.0f;
            binding.m_down = true;
            binding.m_pressed = true;
         }

         if (IsKeyReleased(key)) {
            binding.m_released = true;
         }
      }
   };

   for (auto& it : m_key_actions) {
      key_update(it.data);
   }

   for (auto& it : m_axis_actions) {
      key_update(it.data.m_positive);
      key_update(it.data.m_negative);
   }

   for (auto& it : m_dir_actions) {
      key_update(it.data.m_x.m_positive);
      key_update(it.data.m_x.m_negative);
      key_update(it.data.m_y.m_positive);
      key_update(it.data.m_y.m_negative);
   }
}

Vector2 InputContext::value(const std::string& name) const
{
   for (const auto& it : m_key_actions) {
      if (it.name == name) {
         return Vector2{ it.data.value(), 0.0f };
      }
   }

   for (const auto& it : m_axis_actions) {
      if (it.name == name) {
         return Vector2{ it.data.value(), 0.0f };
      }
   }

   for (const auto& it : m_dir_actions) {
      if (it.name == name) {
         return it.data.value();
      }
   }

   return Vector2{ 0.0f, 0.0f };
}

bool InputContext::down(const std::string& name) const
{
   for (const auto& it : m_key_actions) {
      if (it.name == name) {
         return it.data.down();
      }
   }

   for (const auto& it : m_axis_actions) {
      if (it.name == name) {
         return it.data.down();
      }
   }

   for (const auto& it : m_dir_actions) {
      if (it.name == name) {
         return it.data.down();
      }
   }

   return false;
}

bool InputContext::pressed(const std::string& name) const
{
   for (const auto& it : m_key_actions) {
      if (it.name == name) {
         return it.data.pressed();
      }
   }

   for (const auto& it : m_axis_actions) {
      if (it.name == name) {
         return it.data.pressed();
      }
   }

   for (const auto& it : m_dir_actions) {
      if (it.name == name) {
         return it.data.pressed();
      }
   }

   return false;
}

bool InputContext::released(const std::string& name) const
{
   for (const auto& it : m_key_actions) {
      if (it.name == name) {
         return it.data.released();
      }
   }

   for (const auto& it : m_axis_actions) {
      if (it.name == name) {
         return it.data.released();
      }
   }

   for (const auto& it : m_dir_actions) {
      if (it.name == name) {
         return it.data.released();
      }
   }

   return false;
}

InputContext& InputContext::add_key_binding(const std::string& name, unsigned int scancode)
{
   for (auto& action : m_key_actions) {
      if (action.name == name) {
         action.data.add(scancode);
         return *this;
      }
   }

   m_key_actions.emplace_back(name, KeyBinding{});
   m_key_actions.back().data.add(scancode);

   return *this;
}

InputContext& InputContext::add_axis_binding(const std::string& name, unsigned int negative, unsigned int positive)
{
   for (auto& action : m_axis_actions) {
      if (action.name == name) {
         action.data.add(negative, positive);
         return *this;
      }
   }

   m_axis_actions.emplace_back(name, AxisBinding{});
   m_axis_actions.back().data.add(negative, positive);

   return *this;
}

InputContext& InputContext::add_direction_binding(const std::string& name, unsigned int left, unsigned int right, unsigned int up, unsigned int down)
{
   for (auto& action : m_dir_actions) {
      if (action.name == name) {
         action.data.add(left, right, up, down);
         return *this;
      }
   }

   m_dir_actions.emplace_back(name, DirectionBinding{});
   m_dir_actions.back().data.add(left, right, up, down);

   return *this;
}

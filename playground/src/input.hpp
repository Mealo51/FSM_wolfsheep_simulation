// input.hpp

#pragma once

#include <raylib.h>
#include <vector>
#include <string>

struct KeyBinding {
   KeyBinding() = default;

   float value() const;
   bool down() const;
   bool pressed() const;
   bool released() const;

   void clear();
   KeyBinding& add(unsigned int scancode);

   float m_value{ 0 };
   bool m_down{ false };
   bool m_pressed{ false };
   bool m_released{ false };
   unsigned int m_keys[8] = { 0 };
   unsigned int m_count{ 0 };
};

struct AxisBinding {
   AxisBinding() = default;

   float value() const;
   bool down() const;
   bool pressed() const;
   bool released() const;

   void clear();
   AxisBinding& add(unsigned int negative, unsigned int positive);

   KeyBinding m_negative;
   KeyBinding m_positive;
};

struct DirectionBinding {
   DirectionBinding() = default;

   Vector2 value() const;

   bool down() const;
   bool pressed() const;
   bool released() const;

   void clear();
   DirectionBinding& add(unsigned int left, unsigned int right, unsigned int up, unsigned int down);

   AxisBinding m_x;
   AxisBinding m_y;
};

struct InputContext
{
   template <typename T>
   struct InputAction {
      std::string  name;
      T            data;
   };

   InputContext() = default;

   bool save(const std::string_view path) const;

   void update();

   Vector2 value(const std::string& name) const;
   bool down(const std::string& name) const;
   bool pressed(const std::string& name) const;
   bool released(const std::string& name) const;

   InputContext& add_key_binding(const std::string& name, unsigned int scancode);
   InputContext& add_axis_binding(const std::string& name, unsigned int negative, unsigned int positive);
   InputContext& add_direction_binding(const std::string& name, unsigned int left, unsigned int right, unsigned int up, unsigned int down);

   std::vector<InputAction<KeyBinding>> m_key_actions;
   std::vector<InputAction<AxisBinding>> m_axis_actions;
   std::vector<InputAction<DirectionBinding>> m_dir_actions;
};

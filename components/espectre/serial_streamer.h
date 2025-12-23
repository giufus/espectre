/*
 * ESPectre - Serial Streamer
 * 
 * Streams CSI motion data over USB Serial for external clients.
 * Used by the ESPectre browser game and other real-time applications.
 * 
 * Protocol:
 * - Client -> ESP32: "START\n" to enable, "STOP\n" to disable
 * - ESP32 -> Client: "G<movement>,<threshold>\n" every CSI packet
 * 
 * Author: Francesco Pace <francesco.pace@gmail.com>
 * License: GPLv3
 */

#pragma once

#include <cstdint>
#include <cstddef>

namespace esphome {
namespace espectre {

/**
 * Serial Streamer
 * 
 * Streams movement and threshold data over USB Serial when activated.
 * No overhead when inactive - only listens for START/STOP commands.
 */
class SerialStreamer {
 public:
  /**
   * Initialize serial streamer
   */
  void init();
  
  /**
   * Check for incoming Serial commands
   * 
   * Should be called periodically (e.g., in loop()).
   * Parses "START\n" and "STOP\n" commands.
   */
  void check_commands();
  
  /**
   * Send data over Serial
   * 
   * Sends "G<movement>,<threshold>\n" format.
   * Only sends if streaming is active.
   * 
   * @param movement Current movement intensity (0.0+)
   * @param threshold Current detection threshold
   */
  void send_data(float movement, float threshold);
  
  /**
   * Check if streaming is currently active
   * 
   * @return true if streaming is active (client requested START)
   */
  bool is_active() const { return active_; }
  
  /**
   * Start streaming
   * 
   * Called when "START\n" command is received.
   */
  void start();
  
  /**
   * Stop streaming
   * 
   * Called when "STOP\n" command is received.
   */
  void stop();
  
 private:
  bool active_{false};  // Streaming currently active
  
  // Serial command buffer
  static constexpr size_t CMD_BUFFER_SIZE = 16;
  char cmd_buffer_[CMD_BUFFER_SIZE];
  size_t cmd_buffer_index_{0};
  
  /**
   * Process a complete command line
   * 
   * @param cmd Command string (without newline)
   */
  void process_command_(const char* cmd);
};

}  // namespace espectre
}  // namespace esphome


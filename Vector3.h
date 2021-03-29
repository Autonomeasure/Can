/*
 * CanSat 2020-2021 Team Autonomeasure - Can
 * Code written by: Joep van Dijk
 * First mission:
 *    Our first mission is to be able to land properly, collect data from the BMP280 sensor and send that data to the groundstation at least one time per second. 
 * 
 * Second mission:
 *    Our second mission is to make a sustainable data collection station. 
 * 
 * Links:
 *    GitHub Autonomeasure:           https://github.com/Autonomeasure
 *    GitHub Can repo:                https://github.com/Autonomeasure/Can
 *    GitHub GroundStation repo:      https://github.com/Autonomeasure/GroundStation
 *    Instagram:                      https://instagram.com/Autonomeasure/
 *    
 * This project falls under the GNU GPL-3.0 license, see LICENSE or https://www.gnu.org/licenses/gpl-3.0.txt. 
 */
#if !defined(VECTOR3_H)
#define VECTOR3_H

typedef struct {
  float x;
  float y;
  float z;
}Vector3;

// Subtract two Vector3 objects from each other
//Vector3 minus(Vector3 *a, Vector3 *b) {
//  Vector3 ret;
//  ret.x = a->x - b->x;
//  ret.y = a->y - b->y;
//  ret.z = a->z - b->z;
//  return ret;
//}

#endif

#pragma once

/** @brief Frees and invalidates a pointer.
 *
 *   This is a convenience function freeing and invalidating
 *   a pointer.
 */
#define SAFE_FREE(ptr) do { free(ptr); ptr = NULL; } while (0)

/** @brief Maps a value x from a given range to another range
 *
 *  The input x is mapped from the range in_min and in_max
 *  in relation to the range out_min and out_max.
 *
 *  @return the mapped value
 */
static int map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

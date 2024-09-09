/*
	Copyright (c) 2022 ByteBit/xtreme8000

	This file is part of CavEX.

	CavEX is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	CavEX is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with CavEX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LEVEL_ARCHIVE_H
#define LEVEL_ARCHIVE_H

#include <m-lib/m-string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "cNBT/nbt.h"
#include "cglm/cglm.h"

#include "../item/inventory.h"
#include "../item/items.h"
#include "../world.h"

struct level_archive_tag {
	const char* name;
	nbt_type type;
};

#define LEVEL_NAME                                                             \
	(struct level_archive_tag) {                                               \
		".Data.LevelName", TAG_STRING                                          \
	}
#define LEVEL_TIME                                                             \
	(struct level_archive_tag) {                                               \
		".Data.Time", TAG_LONG                                                 \
	}
#define LEVEL_DISK_SIZE                                                        \
	(struct level_archive_tag) {                                               \
		".Data.SizeOnDisk", TAG_LONG                                           \
	}
#define LEVEL_LAST_PLAYED                                                      \
	(struct level_archive_tag) {                                               \
		".Data.LastPlayed", TAG_LONG                                           \
	}
#define LEVEL_RANDOM_SEED                                                      \
	(struct level_archive_tag) {                                               \
		".Data.RandomSeed", TAG_LONG                                           \
	}
#define LEVEL_PLAYER_HEALTH                                                    \
	(struct level_archive_tag) {                                               \
		".Data.Player.Health", TAG_SHORT                                       \
	}
#define LEVEL_PLAYER_POSITION                                                  \
	(struct level_archive_tag) {                                               \
		".Data.Player.Pos", TAG_LIST                                           \
	}
#define LEVEL_PLAYER_ROTATION                                                  \
	(struct level_archive_tag) {                                               \
		".Data.Player.Rotation", TAG_LIST                                      \
	}
#define LEVEL_PLAYER_VELOCITY                                                  \
	(struct level_archive_tag) {                                               \
		".Data.Player.Motion", TAG_LIST                                        \
	}
#define LEVEL_PLAYER_DIMENSION                                                 \
	(struct level_archive_tag) {                                               \
		".Data.Player.Dimension", TAG_INT                                      \
	}

#define LEVEL_PLAYER_INVENTORY                                                 \
	(struct level_archive_tag) {                                               \
		".Data.Player.Inventory", TAG_LIST                                     \
	}
#define LEVEL_PLAYER_ITEM_ID                                                   \
	(struct level_archive_tag) {                                               \
		".id", TAG_SHORT                                                       \
	}
#define LEVEL_PLAYER_ITEM_COUNT                                                \
	(struct level_archive_tag) {                                               \
		".Count", TAG_BYTE                                                     \
	}
#define LEVEL_PLAYER_ITEM_DURABILITY                                           \
	(struct level_archive_tag) {                                               \
		".Damage", TAG_SHORT                                                   \
	}
#define LEVEL_PLAYER_ITEM_SLOT                                                 \
	(struct level_archive_tag) {                                               \
		".Slot", TAG_BYTE                                                      \
	}

struct level_archive {
	nbt_node* data;
	string_t file_name;
	bool modified;
};

bool level_archive_create(struct level_archive* la, string_t filename);
bool level_archive_read(struct level_archive* la, struct level_archive_tag tag,
						void* result, size_t length);
bool level_archive_write(struct level_archive* la, struct level_archive_tag tag,
						 void* data);
bool level_archive_write_player(struct level_archive* la, vec3 position,
								vec2 rotation, vec3 velocity,
								enum world_dim dimension);
bool level_archive_write_inventory(struct level_archive* la,
								   struct inventory* inventory);
bool level_archive_read_inventory(struct level_archive* la,
								  struct inventory* inventory);
bool level_archive_read_player(struct level_archive* la, vec3 position,
							   vec2 rotation, vec3 velocity,
							   enum world_dim* dimension);
void level_archive_destroy(struct level_archive* la);

#endif

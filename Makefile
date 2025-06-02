# ---------------------------------------------------------------------------- #
#                                  USER CONFIG                                 #
# ---------------------------------------------------------------------------- #


# ---------------------------------- window ---------------------------------- #

WIDTH = 1920 # width of window
HEIGHT = 1080 # hight of window

# --------------------------------- rendering -------------------------------- #

DEFAULT_BOUNCE = 10 # for better result set to hight value
SSAA_FACTOR = 1 # supersampling factor
RAY_RANDOMNESS = 0.02 # add a small random to every ray, help with edge antialiasing

# -------------------------------- preformance ------------------------------- #

MAX_THREAD = 4 # check your cpu for best value
MAX_RAM = 10 # max amount of RAM miniRT can take (a medium render is ~2GB)

# ------------------------------------- - ------------------------------------ #

NAME = miniRT
MODE ?= release
CONFIG = -D WIDTH=$(WIDTH) -D HEIGHT=$(HEIGHT) -D DEFAULT_BOUNCE=$(DEFAULT_BOUNCE) -D SSAA_FACTOR=$(SSAA_FACTOR) -D MAX_THREAD=$(MAX_THREAD) -D MAX_RAM=$(MAX_RAM) -D RAY_RANDOMNESS=$(RAY_RANDOMNESS)
DEBUG_CONFIG = -D WIDTH=$(WIDTH) -D HEIGHT=$(HEIGHT) -D DEFAULT_BOUNCE=1 -D SSAA_FACTOR=1 -D MAX_THREAD=$(MAX_THREAD) -D MAX_RAM=$(MAX_RAM) -D RAY_RANDOMNESS=$(RAY_RANDOMNESS)

OBJ_DIR = obj-$(MODE)
INCLUDES = -Iincludes -Ilibft/includes -Imlx
LIBS = libft/bin/libft.a mlx/libmlx.a

CC = cc
CFLAGS = -Wall -Werror -Wextra -MD -MP $(INCLUDES) $(CONFIG)
MLXFLAGS = -lX11 -lXext -lbsd -lm

ifeq ($(MODE), debug)
	CFLAGS = -Wall -Wextra -MD -MP $(INCLUDES) -g3 $(DEBUG_CONFIG)
endif

VPATH = srcs:srcs/parser:srcs/renderer:srcs/camera:srcs/textures:srcs/button

SRCS =	main.c						\
		utils.c						\
		free.c						\
		init.c						\
		parse.c						\
		parse_objects.c				\
		parse_utils.c				\
		print_struct.c				\
		print_struct_utils.c		\
		parse_material.c			\
		image.c						\
		window.c					\
		renderer.c					\
		ray.c						\
		colors.c					\
		sphere.c					\
		plane.c						\
		cylinder.c					\
		cone.c						\
		lighting.c					\
		pipeline.c					\
		camera.c					\
		cam_transform.c				\
		phong_model.c				\
		intersection.c				\
		input.c						\
		input_utils.c				\
		checker.c					\
		ppm_loader.c				\
		ppm_header.c				\
		ppm_utils.c					\
		effects_complex.c			\
		effects_simple.c			\
		ppm_saver.c					\
		button.c					\
		draw_objects_controls.c		\
		put_string_win.c			\
		put_string_objects.c		\
		put_string_objects_utils.c	\
		put_mat_info.c				\
		handle_button_click.c		\
		handle_button_click_utils.c	\
		handle_mouse_move.c			\
		handle_mouse_move_utils.c	\
		handle_dragging_slider.c	\
		draw.c						\
		draw_utils.c				\
		init_button.c				\
		init_button_utils.c			\
		handle_tabs.c				\
		uv.c						\
		hit.c						\
		depth_of_field.c			\
		renderer_utils.c			\
		antialiaser.c				\

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
DEPS = $(OBJS:.o=.d)
BIN = $(NAME)

RESET			= \033[0m
GRAY			= \033[90m
RED 			= \033[31m
GREEN 			= \033[32m
YELLOW 			= \033[33m
BLUE 			= \033[34m

all:
	$(MAKE) libft
	$(MAKE) mlx
	$(MAKE) $(BIN)
	printf "$(RESET)"

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

libft:
	$(MAKE) -C libft

mlx:
	$(MAKE) -C mlx > /dev/null 2>&1

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@ $(MLXFLAGS)

$(OBJ_DIR)/%.o: %.c Makefile $(LIBS)| $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	printf "$(GRAY)compiling: $(BLUE)%-40s $(GRAY)[%d/%d]\n" "$<" "$$(ls $(OBJ_DIR) | grep -c '\.o')" "$(words $(SRCS))"

clean:
	rm -rf obj-*
	$(MAKE) -C libft clean
	$(MAKE) -C mlx clean > /dev/null 2>&1

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C libft fclean
	$(MAKE) -C mlx clean > /dev/null 2>&1
	rm -f mlx/libmlx.a

re: fclean all

.PHONY: all libft mlx leaks clean fclean re debug release

-include $(DEPS)
.SILENT:
MAKEFLAGS=--no-print-directory

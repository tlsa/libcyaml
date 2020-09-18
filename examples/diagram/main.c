/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2020 Michael Drake <tlsa@netsurf-browser.org>
 */

#include <stdlib.h>
#include <stdio.h>

#include <cyaml/cyaml.h>

struct diagram_point {
	int x;
	int y;
};

struct diagram_circle {
	unsigned radius;
	struct diagram_point centre;
};

struct diagram_polygon {
	struct diagram_point *points;
	unsigned points_count;
};

struct diagram_text {
	char *string;
	struct diagram_point pos;
};

struct diagram_element {
	enum {
		DIAGRAM_SHAPE_CIRCLE,
		DIAGRAM_SHAPE_POLYGON = 9000,
		DIAGRAM_SHAPE_TEXT =15 ,
	} shape;
	union {
		struct diagram_circle circle;
		struct diagram_polygon polygon;
		struct diagram_text text;
	};
};

struct diagram {
	char *creator;
	struct diagram_element *elements;
	unsigned elements_count;
};

/* CYAML mapping schema fields array for . */
static const cyaml_schema_field_t schema_diagram_point_mapping[] = {
	CYAML_FIELD_INT("x", CYAML_FLAG_POINTER, struct diagram_point, x),
	CYAML_FIELD_INT("y", CYAML_FLAG_POINTER, struct diagram_point, y),
	CYAML_FIELD_END
};

/* CYAML value schema for . */
static const cyaml_schema_value_t schema_diagram_point = {
	CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, struct diagram_point,
			schema_diagram_point_mapping),
};

/* CYAML mapping schema fields array for . */
static const cyaml_schema_field_t schema_diagram_circle_mapping[] = {
	CYAML_FIELD_UINT("radius", CYAML_FLAG_DEFAULT,
			struct diagram_circle, radius),
	CYAML_FIELD_MAPPING("centre", CYAML_FLAG_DEFAULT,
			struct diagram_circle, centre,
			schema_diagram_point_mapping),
	CYAML_FIELD_END
};

/* CYAML mapping schema fields array for . */
static const cyaml_schema_field_t schema_diagram_polygon_mapping[] = {
	CYAML_FIELD_SEQUENCE("points", CYAML_FLAG_POINTER,
			struct diagram_polygon, points,
			&schema_diagram_point,
			0, CYAML_UNLIMITED),
	CYAML_FIELD_END
};

/* CYAML mapping schema fields array for . */
static const cyaml_schema_field_t schema_diagram_text_mapping[] = {
	CYAML_FIELD_STRING_PTR("string", CYAML_FLAG_POINTER,
			struct diagram_text, string,
			0, CYAML_UNLIMITED),
	CYAML_FIELD_MAPPING("position", CYAML_FLAG_DEFAULT,
			struct diagram_text, pos,
			schema_diagram_point_mapping),
	CYAML_FIELD_END
};

static const cyaml_strval_t diagram_shapes[] = {
	{ .str = "circle",  .val = DIAGRAM_SHAPE_CIRCLE,  },
	{ .str = "polygon", .val = DIAGRAM_SHAPE_POLYGON, },
	{ .str = "text",    .val = DIAGRAM_SHAPE_TEXT, },
};

/* CYAML mapping schema fields array for . */
static const cyaml_schema_field_t schema_diagram_element_mapping[] = {
	CYAML_FIELD_ENUM("shape", CYAML_FLAG_OPTIONAL,
			struct diagram_element, shape,
			diagram_shapes, CYAML_ARRAY_LEN(diagram_shapes)),
	CYAML_FIELD_MAPPING("circle", CYAML_FLAG_DEFAULT,
			struct diagram_element, circle,
			schema_diagram_circle_mapping),
	CYAML_FIELD_MAPPING("polygon", CYAML_FLAG_DEFAULT,
			struct diagram_element, polygon,
			schema_diagram_polygon_mapping),
	CYAML_FIELD_MAPPING("text", CYAML_FLAG_DEFAULT,
			struct diagram_element, text,
			schema_diagram_text_mapping),
	CYAML_FIELD_END
};

/* CYAML value schema for . */
static const cyaml_schema_value_t schema_diagram_element = {
	CYAML_VALUE_UNION(CYAML_FLAG_DEFAULT, struct diagram_element,
			schema_diagram_element_mapping, "shape"),
};

/* CYAML mapping schema fields array for . */
static const cyaml_schema_field_t schema_diagram_mapping[] = {
	CYAML_FIELD_STRING_PTR("creator", CYAML_FLAG_POINTER,
			struct diagram, creator,
			0, CYAML_UNLIMITED),
	CYAML_FIELD_SEQUENCE("elements", CYAML_FLAG_POINTER,
			struct diagram, elements, &schema_diagram_element,
			0, CYAML_UNLIMITED),
	CYAML_FIELD_END
};

/* CYAML value schema for the top level mapping. */
static const cyaml_schema_value_t schema_diagram = {
	CYAML_VALUE_MAPPING(CYAML_FLAG_POINTER, struct diagram,
			schema_diagram_mapping),
};


/******************************************************************************
 * Actual code to load and save YAML doc using libcyaml.
 ******************************************************************************/

/* Our CYAML config.
 *
 * If you want to change it between calls, don't make it const.
 *
 * Here we have a very basic config.
 */
static const cyaml_config_t config = {
	.log_level = CYAML_LOG_DEBUG, /* Logging errors and warnings only. */
	.log_fn = cyaml_log,            /* Use the default logging function. */
	.mem_fn = cyaml_mem,            /* Use the default memory allocator. */
};

/* Main entry point from OS. */
int main(int argc, char *argv[])
{
	cyaml_err_t err;
	struct diagram *diagram;
	enum {
		ARG_PROG_NAME,
		ARG_PATH_IN,
		ARG_PATH_OUT,
		ARG__COUNT,
	};

	/* Handle args */
	if (argc != ARG__COUNT) {
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "  %s <INPUT> <OUTPUT>\n", argv[ARG_PROG_NAME]);
		return EXIT_FAILURE;
	}

	/* Load input file. */
	err = cyaml_load_file(argv[ARG_PATH_IN], &config,
			&schema_diagram, (void **) &diagram, NULL);
	if (err != CYAML_OK) {
		fprintf(stderr, "ERROR: %s\n", cyaml_strerror(err));
		return EXIT_FAILURE;
	}

	/* Use the data. */
	printf("creator: %s\n", diagram->creator);
	printf("elements:\n");
	for (unsigned i = 0; i < diagram->elements_count; i++) {
		switch (diagram->elements[i].shape) {
		case DIAGRAM_SHAPE_CIRCLE:
			printf("  circle:\n");
			printf("    radius: %u\n", diagram->elements[i].circle.radius);
			printf("      x: %u\n", diagram->elements[i].circle.centre.x);
			printf("      y: %u\n", diagram->elements[i].circle.centre.y);
			break;
		case DIAGRAM_SHAPE_POLYGON:
			printf("  polygon:\n");
			printf("    points:\n");
			for (unsigned j = 0; j < diagram->elements[i].polygon.points_count; j++) {
				printf("      - x: %u\n", diagram->elements[i].polygon.points[j].x);
				printf("        y: %u\n", diagram->elements[i].polygon.points[j].y);
			}
			break;
		case DIAGRAM_SHAPE_TEXT:
			printf("  text:\n");
			printf("    string: %s\n", diagram->elements[i].text.string);
			printf("    position:\n");
			printf("      x: %u\n", diagram->elements[i].text.pos.x);
			printf("      y: %u\n", diagram->elements[i].text.pos.y);
			break;
		default:
			break;
		}
	}

	/* Modify the data */
	diagram->elements[0].circle.centre.x += 100;
	diagram->elements[0].circle.centre.y += 100;

	/* Save data to new YAML file. */
	err = cyaml_save_file(argv[ARG_PATH_OUT], &config,
			&schema_diagram, diagram, 0);
	if (err != CYAML_OK) {
		fprintf(stderr, "ERROR: %s\n", cyaml_strerror(err));
		cyaml_free(&config, &schema_diagram, diagram, 0);
		return EXIT_FAILURE;
	}

	/* Free the data */
	cyaml_free(&config, &schema_diagram, diagram, 0);

	return EXIT_SUCCESS;
}

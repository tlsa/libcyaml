executable('planner', files('main.c'),
		include_directories: include_dir,
		link_with: libcyaml)
configure_file(
	input: files('project.yaml'),
	copy: true,
	output: '@PLAINNAME@')

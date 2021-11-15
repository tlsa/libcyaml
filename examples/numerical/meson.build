executable('numerical', files('main.c'),
		include_directories: include_dir,
		link_with: libcyaml)
configure_file(
	input: files('fibonacci.yaml'),
	copy: true,
	output: '@PLAINNAME@')

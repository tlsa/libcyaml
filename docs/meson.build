project_root = meson.project_source_root()
doxygen_conf = [{
	'name': 'api',
	'conf_template': files('api.doxygen.conf.in'),
	'inputs': ' '.join(
			f'@project_root@/include',
			f'@project_root@/README.md',
			f'@project_root@/docs/guide.md')
},{
	'name': 'devel',
	'conf_template': files('devel.doxygen.conf.in'),
	'inputs': ' '.join(
			f'@project_root@/include',
			f'@project_root@/src',
			f'@project_root@/README.md',
			f'@project_root@/docs/guide.md')
}]

guide = files('guide.md')

if get_option('build_docs')
	doxygen = find_program('doxygen', required: false)
	if doxygen.found()
		doxygen_files = []
		foreach conf_entry: doxygen_conf
			conf_entry_name = conf_entry.get('name')
			doxygen_files += configure_file(
				format: 'cmake@',
				input: conf_entry.get('conf_template'),
				output: '@BASENAME@',
				configuration: {
					'OUTPUT_DIR': f'"docs/@conf_entry_name@"',
					'INPUTS': conf_entry.get('inputs')
				})
		endforeach
		custom_target('docs',
			input: doxygen_files,
			output: 'html',
			depend_files: readme + guide + headers + lib_sources,
			command: [doxygen, '@INPUT@'])
	else
		warning('Documentation requested, but doxygen not found.' +
			'Make sure doxygen is installed')
	endif
endif

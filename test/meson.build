if get_option('build_tests')
	subdir('data')
	subdir('units')
	test_executable = executable('cyaml-test', test_sources,
				      link_with: libcyaml,
				      include_directories: include_dir,
				      c_args: '-DSAVE_FILE_DEST="test/data"')
	foreach unit: test_units
		test_list = []
		foreach entry: unit['test_list']
			if entry.has_key('prefix')
				if entry.has_key('range')
					values = entry['range']
				elif entry.has_key('enum')
					values = entry['enum']
				endif
				if entry.has_key('enum') or entry.has_key('range')
					foreach value: values
						test_list += '@0@@1@'.format(entry['prefix'], value)
					endforeach
				else
					warning('"@0@" in "@1@" doesnt have "enum" nor "range"'
						.format(entry['prefix'], unit['name']))
				endif
			elif entry.has_key('name')
				test_list += entry['name']
			else
				warning('"@0@" has neither "prefix" nor "name"'
					.format(unit['name']))
			endif
		endforeach
		test_list_str = ','.join(test_list)
		test(unit['name'], test_executable,
		     workdir: meson.project_build_root(),
		     args: test_list_str)
	endforeach
endif

project('libcyaml','c',
	default_options: ['c_std=c11'],
	version: '1.2.1-1',
	license: 'ISC',
	meson_version: '>=0.58')

# version format: <major>.<minor>.<patch>[-devel]
version = meson.project_version()
version_arr = version.split('.')
version_major = version_arr[0]
version_minor = version_arr[1]
version_patch = version_arr[2].contains('-') ? version_arr[2].split('-')[0] : version_arr[2]
version_devel = version_arr[2].contains('-') ? version_arr[2].split('-')[1] : '0'

# needed only for .pc file generation
pkg = import('pkgconfig')

libyaml = dependency('yaml-0.1')

version_flags = [
	f'-DVERSION_MAJOR=@version_major@',
	f'-DVERSION_MINOR=@version_minor@',
	f'-DVERSION_PATCH=@version_patch@',
	f'-DVERSION_DEVEL=@version_devel@',
]
add_project_arguments(version_flags, language: 'c')

readme = files('README.md')

headers = files('include/cyaml/cyaml.h')
install_headers(headers, subdir: 'cyaml')

libcyaml_include = include_directories('include')

subdir('src')           # defines 'lib_sources' variable (list[file])

libcyaml = library('cyaml', lib_sources,
		dependencies: libyaml,
		include_directories: libcyaml_include,
		install: true,
		version: '.'.join(version_major, version_minor, version_patch))

subdir('test')		# adds test executable, needs libcyaml defined
subdir('examples')	# adds examples executables, needs libcyaml defined
subdir('docs')		# adds 'docs' custom target

pkg.generate(libcyaml,
	name: 'libcyaml',
	description: 'Schema-based YAML parsing and serialisation',
	url: 'github.com/tlsa/libcyaml')

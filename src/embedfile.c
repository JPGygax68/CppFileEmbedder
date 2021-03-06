#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

static FILE *
open_or_exit(const char* fname, const char* mode)
{
  FILE* f = fopen(fname, mode);
  if (f == NULL) {
    perror(fname);
    exit(EXIT_FAILURE);
  }
  return f;
}

int main(int argc, char** argv)
{
	int i;
	FILE *out = NULL, *in = NULL;
	uint8_t buf[256];
	size_t nread, total;
	size_t linesize;
	size_t j;

	if (argc < 1) {
	fprintf(stderr, "USAGE: %s <resource_files>\n\n"
		"  Makes the binary content of each specified resource file available through a string-index\n"
		"  STL map of strings named \"embedded\"\n", argv[0]);
		return EXIT_FAILURE;
	}

	out = open_or_exit(argv[1], "w");

	fprintf(out, "/* This is a generated file, DO NOT EDIT. */\n");
	fprintf(out, "\n");
	fprintf(out, "#include <map>\n");
	fprintf(out, "#include <string>\n");
	fprintf(out, "#include <cstdint>\n");
	fprintf(out, "#include <array>\n");
	fprintf(out, "\n");
	fprintf(out, "class Embedded {\n");
	fprintf(out, "public:\n");
	fprintf(out, "    struct File {\n");
	fprintf(out, "		size_t _size;\n");
	fprintf(out, "		const char *_raw;\n");
	fprintf(out, "		size_t size() const { return _size; }\n");
	fprintf(out, "		template<typename T> const T *data() const { return static_cast<const T*>(_raw); }\n");
	fprintf(out, "    };\n");
	fprintf(out, "\n");
	fprintf(out, "    Embedded();\n");
	fprintf(out, "\n");
	fprintf(out, "    const File & operator[] (const char *key) {\n");
	fprintf(out, "        return map[key];\n");
	fprintf(out, "    }\n");
	fprintf(out, "\n");
	fprintf(out, "private:\n");
	fprintf(out, "    std::map<std::string, File> map;\n");
	fprintf(out, "};\n");
	fprintf(out, "\n");
	fprintf(out, "#ifndef EMBEDS_EXPORT_ONLY\n");
	fprintf(out, "\n");
	fprintf(out, "Embedded::Embedded() {\n");
	fprintf(out, "    static const struct { const char *key, *content; unsigned int size; } raw[] = {\n");

	for (i = 2; i < argc; i ++) {
    
		fprintf(out, "        { \"%s\",\n          \"", argv[i]);

		in = open_or_exit(argv[i], "rb");
		buf[256];
		total = nread = 0;
		linesize = 0;
		do {
			nread = fread(buf, 1, sizeof(buf), in);
			for (j = 0; j < nread; j++) {
				if (linesize++ == 16) { fprintf(out, "\"\n          \""); linesize = 1; }
				fprintf(out, "\\x%02x", buf[j]);
			}
			total += nread;
		} while (nread > 0);

		fprintf(out, "\", \n");
		fprintf(out, "          %u }", total);
		if (i < (argc-1)) fprintf(out, ",");
		fprintf(out, "\n");

		fclose(in);
	}
  
	fprintf(out, "    };\n");
	fprintf(out, "    for (auto i = 0; i < %d; i++) {\n", argc - 2);
	fprintf(out, "        File file = { raw[i].size, raw[i].content };\n");
	fprintf(out, "        map[raw[i].key] = file;\n");
	fprintf(out, "    }\n");
	fprintf(out, "}\n");
	fprintf(out, "\n");
	fprintf(out, "Embedded embeds;\n");

	fprintf(out, "\n");
	fprintf(out, "#else\n");
	fprintf(out, "\n");
	fprintf(out, "extern Embedded embeds;\n");
	fprintf(out, "\n");
	fprintf(out, "#endif\n");
	fprintf(out, "\n");
  
	return EXIT_SUCCESS;
}

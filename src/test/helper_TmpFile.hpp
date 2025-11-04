#ifndef HELPER_TMPFILE_HPP
# define HELPER_TMPFILE_HPP

# include <fstream>
# include <string>

class TmpPegFile {
private:
	std::string _path;

public:
	TmpPegFile(const std::string &content, const std::string &filename = "tmpFile.peg") {
		_path = "./" + filename;
		std::ofstream ofs(_path.c_str());
		if (!ofs.is_open())
			throw std::runtime_error("Failed to create temp .peg file");
		ofs << content;
		ofs.close();
	}

	~TmpPegFile() {
		std::remove(_path.c_str());
	}

	const std::string &path() const { return _path; }
};

#endif


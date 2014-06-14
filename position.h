#ifndef POSITION_H
#define POSITION_H
	
	struct Position {
	int line, linePosition;

	Position (int line = 0, int linePosition = 0): line(line), linePosition(linePosition){

	} 

	std::string toString(){
		std::stringstream ss;
		ss << "line: " << this->line << ", position: " << this->linePosition;
		return ss.str();
	}
};
#endif
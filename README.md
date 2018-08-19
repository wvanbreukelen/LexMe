# LexMe
A simple, but customizable lexer, used for the three stage interpreter pipeline

## Classes and their responsablities
- Lexer: public interface to the end user. This class will call other composite classes (for example call ```LexeralAnalysis```)
- LexeralAnalysis: performs the lexeral analysis.
- TokenData: data container for a token
- Token: typedef shared pointer of ```TokenData```
- TokenList: Container for tokens, inherits ```std::vector```.
- TokenType: Types of tokens available (such as operators, newlines, function indentifiers, etc.)

## Language Definition
As the lexer needs input to correctly parse tokens, the language characteristics need to be defined. These characteristics are defined in the ```LanguageDefinition.h``` header file.

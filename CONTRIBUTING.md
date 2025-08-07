# Contributing to AlertSysML

Thank you for your interest in contributing to the AlertSysML library! We welcome contributions from the community to help improve this intelligent sensor monitoring system.

## How to Contribute

### Reporting Bugs
If you find a bug, please create an issue with:
- **Clear description** of the problem
- **Steps to reproduce** the issue
- **Expected vs actual behavior**
- **Hardware details** (ESP32 model, Arduino IDE version)
- **Code snippet** that demonstrates the issue
- **Serial output** if applicable

### Suggesting Features
We welcome feature suggestions! Please:
- Check existing issues to avoid duplicates
- Describe the use case and benefit
- Provide examples of how it would work
- Consider backward compatibility

### Contributing Code

#### Before You Start
- Fork the repository
- Create a new branch for your feature/fix
- Check existing issues and pull requests

#### Development Guidelines

**Code Style:**
- Follow existing code formatting and naming conventions
- Use meaningful variable names
- Add comments for complex logic
- Keep functions focused and concise

**Testing:**
- Test your changes on actual ESP32 hardware
- Run the provided test example (`examples/test_all_functions.ino`)
- Test with different sensor values and scenarios
- Verify SPIFFS functionality works correctly

**Documentation:**
- Update README.md if adding new features
- Update function documentation in header files
- Add code examples for new functionality
- Update version history if applicable

#### Pull Request Process

1. **Create Quality Commits**
   - Make atomic commits with clear messages
   - Use present tense ("Add feature" not "Added feature")
   - Reference issue numbers when applicable

2. **Test Thoroughly**
   - Compile successfully with Arduino IDE
   - Test on ESP32 hardware
   - Verify no regression in existing functionality

3. **Submit Pull Request**
   - Provide clear description of changes
   - Link to related issues
   - Include testing details
   - Be responsive to feedback

#### Code Requirements

**Memory Management:**
- Minimize memory usage
- Avoid memory leaks
- Use appropriate data types

**Error Handling:**
- Handle edge cases gracefully
- Provide meaningful error messages
- Validate input parameters

**Compatibility:**
- Maintain ESP32 compatibility
- Test with different Arduino IDE versions
- Consider different SPIFFS configurations

### Areas for Contribution

**High Priority:**
- Bug fixes and stability improvements
- Performance optimizations
- Memory usage improvements
- Additional sensor type support

**Medium Priority:**
- Enhanced pattern recognition algorithms
- Web interface improvements
- Additional storage backends
- More comprehensive examples

**Low Priority:**
- Code refactoring
- Documentation improvements
- Additional utility functions

### Development Setup

1. **Required Hardware:**
   - ESP32 development board
   - Temperature sensor (or simulation capability)
   - USB cable for programming

2. **Software Requirements:**
   - Arduino IDE 1.8.x or newer
   - ESP32 board package
   - SPIFFS support enabled

3. **Getting Started:**
   ```bash
   git clone https://github.com/yourusername/alertsysml.git
   cd alertsysml
   # Open examples/test_all_functions.ino in Arduino IDE
   ```

### Communication

- **Issues:** Use GitHub Issues for bug reports and feature requests
- **Discussions:** Use GitHub Discussions for questions and ideas
- **Pull Requests:** Use PR comments for code-specific discussions

### Code of Conduct

This project follows a Code of Conduct. By participating, you agree to maintain a respectful and inclusive environment for all contributors.

### Recognition

Contributors will be:
- Listed in the repository contributors
- Mentioned in release notes for significant contributions
- Credited in documentation for major features

### Questions?

If you have questions about contributing:
- Check existing issues and documentation
- Create a new issue with the "question" label
- Start a discussion in GitHub Discussions

## Development Tips

### Testing Your Changes
- Always test with the provided example sketch
- Try various sensor values (normal, abnormal, extreme)
- Test pattern learning over simulated multiple days
- Verify SPIFFS operations work correctly
- Test error conditions and edge cases

### Common Pitfalls
- Not handling empty string returns properly
- Memory leaks in vector operations
- SPIFFS file handling errors
- Time boundary conditions (hour/minute validation)
- Float precision issues in comparisons

Thank you for helping make AlertSysML better! 

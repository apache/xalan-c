# Xalan-C++ Command-Line Utilities

## Initial setup

To perform a transformation, you can call Xalan-C++ from the command
line (or script), or from within an application (see
[Usage Patterns](usagepatterns.md)).

To perform a transformation from the command line or a script, do the following:

1. [Download](download.md) and [build](build.md) Xalan-C++, or
   [install](install.md) Xalan-C++ from a third-party distributor.
2. Set the path and/or library path to include the Xalan executable
   and the Xalan-C++ and Xerces-C++ libraries, respectively.
3. Call the Xalan executable with the appropriate flags and arguments
   (described below).

## Using the `Xalan` executable

Call the `Xalan` executable as follows:

```sh
Xalan [options] xmlSource xslStylesheet
```

where *xmlSource* is the XML source document, *xslStylesheet* is the
stylesheet to apply, and *options* are any (or none) of the following:

| Option         | Description                                                            |
|----------------|------------------------------------------------------------------------|
| *-a*           | Use stylesheet processing instruction, not the stylesheet argument.    |
| *-e encoding*  | Force the specified encoding for the output.                           |
| *-i integer*   | Indent the specified amount.                                           |
| *-m*           | Omit the META tag in HTML output.                                      |
| *-o filename*  | Write transformation result to this file (rather than to the console). |
| *-p name expr* | Set a stylesheet parameter with this expression.                       |
| *-t name expr* | Display timing information.                                            |
| *-u name expr* | Disable escaping of URLs in HTML output.                               |
| *-v*           | Validate the XML source document.                                      |
| *-*            | A dash as the *xmlSource* argument reads from stdin.<br>A dash as the *xmlStylesheet* argument reads from stdin.<br/>("-" cannot be used for both arguments.) |

To display a list of the options call the Xalan executable as follows:

```sh
Xalan -?
```

# Downloading Xalan-C++

Xalan-C++ may be obtained in two ways:

1. Binary distribution.
2. Source distribution.

## Binary distribution

See the [Installing Xalan-C++](install.md) chapter for further details.

## Source distribution

The Xalan-C++ sources may be downloaded from:

* [An Apache mirror](http://www.apache.org/dyn/closer.cgi/xalan/xalan-c/sources)
  of the official release files.
* GitHub [releases](https://github.com/apache/xalan-c/releases) of the same
  files.
* GitHub [tags and branches](https://github.com/apache/xalan-c)
  downloads.
* Apache GitBox
  [source browser](https://gitbox.apache.org/repos/asf?p=xalan-c.git)
  or direct Git URI:
  `https://gitbox.apache.org/repos/asf/xerces-c.git`

Note: You must [verify the integrity](#verify-the-integrity-of-the-files)
of the downloaded files using signatures downloaded from the Apache
master distribution directory.

Note: To access previous releases of Xalan-C++, visit the
[Apache Xalan-C++ archives](http://archive.apache.org/dist/xml/xalan-c/).
To access previous releases of Xerces-C++, visit the
[Apache Xerces-C++ archives](http://archive.apache.org/dist/xerces/c/)

After downloading the sources, proceed to
[Building Xalan-C++](build.md) to build Xalan-C++

## Verify The Integrity of the Files

It is essential that you verify the integrity of the downloaded files
using the PGP or MD5 signatures. You should only download the
[KEYS](http://www.apache.org/dist/xalan/xalan-c/KEYS)
and the (asc/md5) signature files from the Apache Master Distribution.
The Apache Mirrors of previous sections should be used to download the
products.

* [Apache Mirror Xalan-C Sources](http://www.apache.org/dyn/mirrors/mirrors.cgi/xalan/xalan-c/sources)
* [Apache Master Xalan-C Sources - for Signature Files](http://www.apache.org/dist/xalan/xalan-c/sources)

Make sure you get the signature file for the particular distribution
from the master directory, rather than from a mirror.  Then verify the
signatures using:

```sh
   % pgpk -a KEYS
   % pgpv <archive-name>.tar.gz.asc
```

*or*

```sh
   % pgp -ka KEYS
   % pgp <archive-name>.tar.gz.asc
```

*or*

```sh
   % gpg --import KEYS
   % gpg --verify <archive-name>.tar.gz.asc
```

Alternatively, you can verify the MD5 signature on the files.

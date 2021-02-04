// oem_tls_test.cpp : 定义控制台应用程序的入口点。
//

#include <xdl.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

char test_ca_crt[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIIDpTCCAo2gAwIBAgIBADANBgkqhkiG9w0BAQUFADBFMQswCQYDVQQGEwJGUjEO\r\n"
"MAwGA1UEBxMFUGFyaXMxDjAMBgNVBAoTBVh5U1NMMRYwFAYDVQQDEw1YeVNTTCBU\r\n"
"ZXN0IENBMB4XDTA3MDcwNzA1MDAxOFoXDTE3MDcwNzA1MDAxOFowRTELMAkGA1UE\r\n"
"BhMCRlIxDjAMBgNVBAcTBVBhcmlzMQ4wDAYDVQQKEwVYeVNTTDEWMBQGA1UEAxMN\r\n"
"WHlTU0wgVGVzdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAM+k\r\n"
"gt70fIPiYqmvXr+9uPmWoN405eoSzxdiRLLCHqL4V/Ts0E/H+JNfHS4DHlAgxrJu\r\n"
"+ZIadvSJuHkI6e1iMkAh5SU1DqaF3jrrFdJCooM6a077M4CRkE1tdAeZDf+BYp0q\r\n"
"BeMU9Y2+j7ibsQaPAizunbXLf4QdteExCwhYRJ8OVXSEaSNt339gJzTD6kOhES3b\r\n"
"lEN3qbx6lqFaJ5MLHTix5uNVc2rvbOizV5oLhJNqm52AKOp11tv6WTiI8loagvAc\r\n"
"jlhEZRNb9el5SL6Jai/uFcqXKzfXNKW3FYPQHFGobmiMfGt1lUSBJ3F2mrqEC7gC\r\n"
"wHy/FDvAI64/k5LZAFkCAwEAAaOBnzCBnDAMBgNVHRMEBTADAQH/MB0GA1UdDgQW\r\n"
"BBS87h+Y6Porg+SkfV7DdXKTMdkyZzBtBgNVHSMEZjBkgBS87h+Y6Porg+SkfV7D\r\n"
"dXKTMdkyZ6FJpEcwRTELMAkGA1UEBhMCRlIxDjAMBgNVBAcTBVBhcmlzMQ4wDAYD\r\n"
"VQQKEwVYeVNTTDEWMBQGA1UEAxMNWHlTU0wgVGVzdCBDQYIBADANBgkqhkiG9w0B\r\n"
"AQUFAAOCAQEAIHdoh0NCg6KAAhwDSmfEgSbKUI8/Zr/d56uw42HO0sj/uKPQzUco\r\n"
"3Mx2BYE1m1itg7q5OhrkB7J4ZB78EtNZM84nV+y6od3YpR0Z9VUxCx7948MozYRy\r\n"
"TKF5x/lKHRx1PJKfEO4clKdWTFAtWtGhewXrHJQ8C+ENh2Up2wTVh3Z+pEzuZNv3\r\n"
"u/JYu1H+vkt3l1WCy/9mxUnu+anW1DzxPWnjy4lx6Mi0BD2qfKBWLjVS+7v6ALcj\r\n"
"S2oRWWr4LUvXT7z9BBAvw2eJQD+a4uAya6EURG7AsAvr5MnWn/r0wLWmBJ6fB1Yp\r\n"
"F1kOmamOFvstLMf74rLX+LGKeJ/nwuI5FQ==\r\n"
"-----END CERTIFICATE-----\r\n";

char test_ca_key[] =
"-----BEGIN RSA PRIVATE KEY-----\r\n"
"Proc-Type: 4,ENCRYPTED\r\n"
"DEK-Info: DES-EDE3-CBC,7BDC280BA4C2F45C\r\n"
"\r\n"
"KI6rmwY0ChjF/0/Q7d1vE2jojZQTuF8j0wJUOf02PUUD4en8/FCLj69Pf3/lvOlu\r\n"
"F8hU4IuBHxN2+feuXp6xTmGd/VyKdWh4+NGtKr2V1gXfk2wqHk3/P/YuF7QhOQjZ\r\n"
"BlNF7n5o76Nufr3iwBbtIABCGQfNRUwGzrvmFXLIrSqns8ppv83qaD5jHVQQj5MM\r\n"
"cGIoidwmNORdVmBCVZ17dQ+lMPfqjhN27GBhzXbp9a2EP3w+IqrXOvcl4DqSY+DU\r\n"
"PIhS6KuQZ4iek4dI93wmw2D8Q67omcKYOX/BjCkZ/v6oq481qOej6MicIY/L9LxH\r\n"
"r91jqIYLJC+1w20YavB3kIkSe5zys30RzhPTOtG60j8kgiQ4Fh+L/nKBP5noOXGE\r\n"
"uzDBa051HCEYfufOVkXr6wBCFo9boqM1p/GI0Ze5gCsYY+Vzyu96gu+OGv55Jtyo\r\n"
"fY2yEwVKhfqNy+xJ+8dwf8dcc5vQLatXebDynuSPI2jbaos6MJT6n0LoY2TpAz47\r\n"
"vNE7UtEEdgZPPVwE4xO+dVa0kCqJxuG8b+ZZTHZidlQ6MBiebL4vXbeIFieQRzUM\r\n"
"06IQ7YqsiBVLYxicMlApdFpJ4QM2fFKlZRo+fHg8EN9HEbRpgxIf4IwAAFjOgsiO\r\n"
"ll7OmyzSF12NUIrRsIo02E4Y8X6xSFeYnOIiqZqdxG4xz/DZKoX6Js8WdYbtzrDv\r\n"
"7gYYEZy1cuR9PJzMDKXoLz/mjBqDsh11Vgm1nAHbhBqFaWSuueH/LV2rgRijUBKh\r\n"
"yMTnGXrz56DeJ85bYLjVEOM3xhvjsz6Fq5xLqXRTAzyQn7QuqRg2szLtCnN8olCh\r\n"
"rdS1Gd7EV5g24WnF9gTzYJ4lwO9oxOsPKKCD1Z77B+lbZLoxTu7+rakTtqLikWMv\r\n"
"7OILfR4iaIu1nKxhhXpwnze3u+1OWcuk0UnBjSnF519tlrgV3dmA0P2LHd6h6xru\r\n"
"vIgUFHbigCV0i55peEPxtcy9JGLGJ3WvHA3NGSsqkkB/ymdfEaMmEH7403UKqQqq\r\n"
"+K9Z1cYmeZlfoXClmdSjsxkYeN5raB0kOOSV/MEOySG+lztyLUGB4n46AZG2PgXN\r\n"
"A9g9tv2gqc51Vl+55qwTg5429DigjiByRkcmBU3A1aF8yzD1QerwobPmqeNZ0mWv\r\n"
"SllAvgVs2uy2Wf9/5gEWm+HjnJwS9VPNTocG/r+4BnDK8XG0Jy4oI+jSPxj/Y8Jt\r\n"
"0ejFPM5A7HGngiaPFYHxcwPwo4Aa4HZnk+keFrA+vF9eDd1IOj195a9TESL+IGt1\r\n"
"nuNmmuYjyJqM9Uvi1Mutv0UQ6Fl6yv4XxaUtMZKl4LtrAaMdW1T0PEgUG0tjSIxX\r\n"
"hBd1W+Ob4nmK29aa4iuXaOxeAA3QK7KCZo18CJFgnp1w97qohwlKf+4FuNXHL64Q\r\n"
"FgmpycV9nfP8G9aUYKAxs1+xutNv/B6opHmfLNL6d0cwi8dvGsrDdGlcyi8Dk/PF\r\n"
"GKSqlQTF5V7l4UOanefB51tuziEBY+LWcXP4XgqNGPQknPF90NnbH1EglQG2paqb\r\n"
"5bLyT8G6kfCSY4uHxs9lPuvfOjk9ptjy2FwfyBb3Sl4K+IFEE8XTNuNErh83AKh2\r\n"
"-----END RSA PRIVATE KEY-----\r\n";

char test_ca_pwd[] = "test";

char test_srv_crt[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIIDPjCCAiagAwIBAgIBAjANBgkqhkiG9w0BAQUFADBFMQswCQYDVQQGEwJGUjEO\r\n"
"MAwGA1UEBxMFUGFyaXMxDjAMBgNVBAoTBVh5U1NMMRYwFAYDVQQDEw1YeVNTTCBU\r\n"
"ZXN0IENBMB4XDTA3MDcwNzA1MDEyOVoXDTA4MDcwNjA1MDEyOVowMTELMAkGA1UE\r\n"
"BhMCRlIxDjAMBgNVBAoTBVh5U1NMMRIwEAYDVQQDEwlsb2NhbGhvc3QwggEiMA0G\r\n"
"CSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC40PDcGTgmHkt6noXDfkjVuymjiNYB\r\n"
"gjtiL7uA1Ke3tXStacEecQek/OJxYqYr7ffcWalS29LL6HbKpi0xLZKBbD9ACkDh\r\n"
"1Z/SvHlyQPILJdYb9DMw+kzZds5myXUjzn7Aem1YjoxMZUAMyc34i2900X2pL0v2\r\n"
"SfCeJ9Ym4MOnZxYl217+dX9ZbkgIgrT6uY2IYK4boDwxbTcyT8i/NPsVsiMwtWPM\r\n"
"rnQMr+XbgS98sUzcZE70Pe1TlV9Iy8j/8d2OiFo+qTyMu/6UpM2s3gdkQkMzx+Sm\r\n"
"4QitRUjzmEXeUePRUjEgHIv7vz069xuVBzrks36w5BXiVAhLke/OTKVPAgMBAAGj\r\n"
"TTBLMAkGA1UdEwQCMAAwHQYDVR0OBBYEFNkOyCTx64SDdPySGWl/tzD7/WMSMB8G\r\n"
"A1UdIwQYMBaAFLzuH5jo+iuD5KR9XsN1cpMx2TJnMA0GCSqGSIb3DQEBBQUAA4IB\r\n"
"AQBelJv5t+suaqy5Lo5bjNeHjNZfgg8EigDQ7NqaosvlQZAsh2N34Gg5YdkGyVdg\r\n"
"s32I/K5aaywyUbG9qVXQxCM2T95qBqyK56h9yJoZKWQD9H//+zB8kCK/16WvRfv3\r\n"
"VA7eSR19qOFWlHe+1qGh2YhxeDUfyi+fm4D36dGxqC2A34tZjo0QPHKtIeqM0kJy\r\n"
"zzL65TlbJQKkyTuRHofFv0jW9ZFG2wkGysVgCY5fjuLI1do/sWUaXd2987iNFa+K\r\n"
"FrHsTi6urSfZuGlZNxDXDHEE7Q2snAvvev+KR7DD9X4DJGcPX9gA4CGJj+9ZzyAA\r\n"
"ZTGpOzk1hIH44RFs2lJMZRlE\r\n"
"-----END CERTIFICATE-----\r\n";

char test_srv_key[] =
"-----BEGIN RSA PRIVATE KEY-----\r\n"
"MIIEowIBAAKCAQEAuNDw3Bk4Jh5Lep6Fw35I1bspo4jWAYI7Yi+7gNSnt7V0rWnB\r\n"
"HnEHpPzicWKmK+333FmpUtvSy+h2yqYtMS2SgWw/QApA4dWf0rx5ckDyCyXWG/Qz\r\n"
"MPpM2XbOZsl1I85+wHptWI6MTGVADMnN+ItvdNF9qS9L9knwnifWJuDDp2cWJdte\r\n"
"/nV/WW5ICIK0+rmNiGCuG6A8MW03Mk/IvzT7FbIjMLVjzK50DK/l24EvfLFM3GRO\r\n"
"9D3tU5VfSMvI//HdjohaPqk8jLv+lKTNrN4HZEJDM8fkpuEIrUVI85hF3lHj0VIx\r\n"
"IByL+789OvcblQc65LN+sOQV4lQIS5HvzkylTwIDAQABAoIBABeah8h0aBlmMRmd\r\n"
"+VN4Y3D4kF7UcRCMQ21Mz1Oq1Si/QgGLyiBLK0DFE16LzNE7eTZpNRjh/lAQhmtn\r\n"
"QcpQGa/x1TomlRbCo8DUVWZkKQWHdYroa0lMDliPtdimzhEepE2M1T5EJmLzY3S+\r\n"
"qVGe7UMsJjJfWgJAezyXteANQK+2YSt+CjPIqIHch1KexUnvdN9++1oEx6AbuZ8T\r\n"
"4avhFYZQP15tZNGsk2LfQlYS/NfbowkCsd0/TVubJBmDGUML/E5MbxjxLzlaNB2M\r\n"
"V59cBNgsgA35CODAUF4xOyoSfZGqG1Rb9qQrv1E6Jz56dG8SsKF3HqnDjxiPOVBN\r\n"
"FBnVJ+ECgYEA29MhAsKMm4XqBUKp6pIMFTgm/s1E5vxig70vqIiL+guvBhhQ7zs1\r\n"
"8UMTNXZoMELNoB/ev9fN0Cjc1Vr46b/x/yDw7wMb96i+vzENOzu4RHWi3OWpCPbp\r\n"
"qBKEi3hzN8M+BulPX8CDQx3aLRrfxw51J5EuA0NeybngbItgxTi0u6kCgYEA1zr0\r\n"
"6P5YdOhYHtSWDlkeD49MApcVuzaHnsHZVAhUqu3Rwiy9LRaJLZfr7fQDb9DYJbZp\r\n"
"sxTRLG6LSAcsR7mw+m+GvNqGt/9pSqbtW+L/VwVWSyF+YYklxZUD3UAAyrDVcDEC\r\n"
"a5S+jad4Csi/lVHt5ulWIckWL1fJvadn5ubKNDcCgYA+71xVGPP+lsFgTiytfrC8\r\n"
"5n2rl4MxinJ9+w0I+EbzCKNMYGvTgiU4dJasSMEdiBKs1FMGo7dF8F0BLHF1IsIa\r\n"
"5Ah2tXItXn9154o9OiTQXMmK6qmRaneM6fhOoeaCwYAhpGxYIpqx/Xr4TOhiag46\r\n"
"jMMaphAeOvw4t1K2RDziOQKBgQCyPCCU0gxuw/o1jda2CxbZy9EmU/erEX09+0n+\r\n"
"TOfQpSEPq/z9WaxAFY9LfsdZ0ZktoeHma1bNdL3i6A3DWAM3YSQzQMRPmzOWnqXx\r\n"
"cgoCBmlvzkzaeLjO5phMoLQHJmmafvuCG6uxov3F8Hi3LyHUF2c8k0nL6ucmJ3vj\r\n"
"uzu4AQKBgBSASMAJS63M9UJB1Eazy2v2NWw04CmzNxUfWrHuKpd/C2ik4QKu0sRO\r\n"
"r9KnkDgxxEhjDm7lXhlW12PU42yORst5I3Eaa1Cfi4KPFn/ozt+iNBYrzd8Tyvnb\r\n"
"qkdECl0+G2Fo/ER4NRCv7a24WNEsOMGzGRqw5cnSJrjbZLYMaIyK\r\n"
"-----END RSA PRIVATE KEY-----\r\n";

char test_cli_crt[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIIDPTCCAiWgAwIBAgIBATANBgkqhkiG9w0BAQUFADBFMQswCQYDVQQGEwJGUjEO\r\n"
"MAwGA1UEBxMFUGFyaXMxDjAMBgNVBAoTBVh5U1NMMRYwFAYDVQQDEw1YeVNTTCBU\r\n"
"ZXN0IENBMB4XDTA3MDcwNzA1MDEyMFoXDTA4MDcwNjA1MDEyMFowMDELMAkGA1UE\r\n"
"BhMCRlIxDjAMBgNVBAoTBVh5U1NMMREwDwYDVQQDEwhKb2UgVXNlcjCCASIwDQYJ\r\n"
"KoZIhvcNAQEBBQADggEPADCCAQoCggEBAKZkg8ANl6kGmLGqKc6KUHb9IsZwb2+K\r\n"
"jBw83Qb0KuvPVnu3MzEcXFvOZ83g0PL/z8ob5PKr8HP6bVYzhsD65imcCDEEVPk2\r\n"
"9r0XGTggGjB601Fd8aTShUWE4NLrKw6YNXTXgTNdvhHNxXwqmdNVLkmZjj3ZwYUc\r\n"
"cEE8eE5jHs8cMDXJLMCwgKIM7Sax22OhSHQHKwifVO4/Fdw5G+Suys8PhMX2jDXM\r\n"
"ICFwq8ld+bZGoNUtgp48FWhAMfJyTEaHh9LC46KkqGSDRIzx7/4cPB6QqrpzJN0o\r\n"
"Kr8kH7vdRDTFDmO23D4C5l0Bw/2aC76DhEJpB2bGA4iIszJs+F/PIL8CAwEAAaNN\r\n"
"MEswCQYDVR0TBAIwADAdBgNVHQ4EFgQUiWX1IvjRdYGt0zz5Sq16x01k0o4wHwYD\r\n"
"VR0jBBgwFoAUvO4fmOj6K4PkpH1ew3VykzHZMmcwDQYJKoZIhvcNAQEFBQADggEB\r\n"
"AGdqD7VThJmC+oeeMUHk2TQX2wZNU+GsC+RLjtlenckny95KnljGvMtCznyLkS5D\r\n"
"fAjLKfR1No8pk5gRdscqgyIuQx5WnHNv4QBZmMsmvDICxzRQaxuPFHbS4aLXldeL\r\n"
"yOWm5Z4qkMHpCKvA86blYsEkksGDV47fF9ZkOQ8nkh7Z4eY4/5TwqTY72ww5g4NL\r\n"
"6DZtWpcpGbX99NRaNVzcq9D+ElxkgHnH4YWafOKBclSgqrutbRLi2uZx/QpvuF+i\r\n"
"sUbe+HFPMWwU5lBv/oOhQkz0VD+HusYtXWS2lG88cT40aNly2CkYUugdTR/b9Uea\r\n"
"p/i862sL/lO40qlQ0xV5N7U=\r\n"
"-----END CERTIFICATE-----\r\n";

char test_cli_key[] =
"-----BEGIN RSA PRIVATE KEY-----\r\n"
"MIIEowIBAAKCAQEApmSDwA2XqQaYsaopzopQdv0ixnBvb4qMHDzdBvQq689We7cz\r\n"
"MRxcW85nzeDQ8v/Pyhvk8qvwc/ptVjOGwPrmKZwIMQRU+Tb2vRcZOCAaMHrTUV3x\r\n"
"pNKFRYTg0usrDpg1dNeBM12+Ec3FfCqZ01UuSZmOPdnBhRxwQTx4TmMezxwwNcks\r\n"
"wLCAogztJrHbY6FIdAcrCJ9U7j8V3Dkb5K7Kzw+ExfaMNcwgIXCryV35tkag1S2C\r\n"
"njwVaEAx8nJMRoeH0sLjoqSoZINEjPHv/hw8HpCqunMk3SgqvyQfu91ENMUOY7bc\r\n"
"PgLmXQHD/ZoLvoOEQmkHZsYDiIizMmz4X88gvwIDAQABAoIBAE0nBkAjDVN+j4ax\r\n"
"1DjEwZKqxVkmAUXDBDyDrCjxRoWY2gz7YW1ALUMUbeV0fO5v1zVrwbkUKKZeVBxI\r\n"
"QA9zRw28H8A6tfvolHgRIcx4dixMh3ePC+DVDJ6zglvKV2ipAwBufKYIrX0r4Io2\r\n"
"ZqUrNg9CeEYNlkHWceaN12rhYwO82pgHxnB1p5pI42pY7lzyLgSddf5n+M5UBOJI\r\n"
"gsNCkvbGdv7WQPVFTRDiRgEnCJ3rI8oPSK6MOUWJw3rh2hbkx+ex8NPvEKbzEXiU\r\n"
"p5j1AlbHIWP5sYBbA1YviFtryAV4fyfLcWPfoqa33Oozofjlwoj0Aixz+6rerLjZ\r\n"
"cpTSrAECgYEA2oCffUo6HH3Lq9oeWhFCoOyG3YjZmFrJaJwjHnvroX9/pXHqYKog\r\n"
"TehcjUJBtFZw0klcetYbZCFqT8v9nf0uPlgaiVGCtXf1MSbFXDUFKkYBiFwzdWMT\r\n"
"Ysmvhff82jMWZ8ecsXTyDRl858SR5WPZ52qEsCc5X2un7QENm6FtVT8CgYEAwvKS\r\n"
"zQNzuoJETqZX7AalmK3JM8Fdam+Qm5LNMcbvhbKwI8HKMS1VMuqaR0XdAX/iMXAx\r\n"
"P1VhSsmoSDbsMpxBEZIptpCen/GcqctITxANTakrBHxqb2aQ5EEu7SzgfHZWse3/\r\n"
"vQEyfcFTBlPIdcZUDzk4/w7WmyivpYtCWoAh1IECgYEA0UYZ+1UJfVpapRj+swMP\r\n"
"DrQbo7i7t7lUaFYLKNpFX2OPLTWC5txqnlOruTu5VHDqE+5hneDNUUTT3uOg4B2q\r\n"
"mdmmaNjh2M6wz0e0BVFexhNQynqMaqTe32IOM8DFs3L0xacgg7JfVn6P7CeQGOVe\r\n"
"wc96kICw6ZxhtJSqpOGipt8CgYBI/0Pw+IXxJK4nNSpe+u4vCYP5mUI9hKEFYCbt\r\n"
"qKwvyAUknn/zgiIQ+r/iSErFMPmlwXjvWi0gL/qPb+Fp4hCLX8u2zNhY08Px4Gin\r\n"
"Ej+pANtWxq+kHyfKEI5dyRwV/snfvlqwjy404JsSF3VMhIMdYDPzbb72Qnni5w5l\r\n"
"jO0eAQKBgBqt9jJMd1JdpemC2dm0BuuDIz2h3/MH+CMjfaDLenVpKykn17B6N92h\r\n"
"klMesqK3RQzDGwauDw431LQw0R69onn9fCM3wJw2yEC6wC9sF8I8hsNZbt64yZhZ\r\n"
"4Bi2YRTiHhpEuBqKlhHLDFHneo3SMYh8PU/PDQQcyWGHHUi9z1RE\r\n"
"-----END RSA PRIVATE KEY-----\r\n";

char xyssl_ca_crt[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIID4DCCAsigAwIBAgIJAOLw9BMV1jxMMA0GCSqGSIb3DQEBBQUAMFMxCzAJBgNV\r\n"
"BAYTAkZSMQ4wDAYDVQQIEwVQYXJpczEOMAwGA1UEChMFWHlTU0wxJDAiBgNVBAMT\r\n"
"G1h5U1NMIENlcnRpZmljYXRlIEF1dGhvcml0eTAeFw0wNjEwMzEyMjU5MjRaFw0x\r\n"
"NjEwMzEyMjU5MjRaMFMxCzAJBgNVBAYTAkZSMQ4wDAYDVQQIEwVQYXJpczEOMAwG\r\n"
"A1UEChMFWHlTU0wxJDAiBgNVBAMTG1h5U1NMIENlcnRpZmljYXRlIEF1dGhvcml0\r\n"
"eTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKnprf1RQ7IYPI3FmI/h\r\n"
"f2EJGfaIP+Jt551VZWFrs3A56Nn4KS57zTAKZUA7YGbLwlAznfaphJ7SvENALeZR\r\n"
"/J0c/n9jwMpfXReQL7RVpgg/zlR+t2DUi3DAwigPZiHHCSJSBC73vpMc6uH0eV2d\r\n"
"itqjjUnJG5F8Zg9/gX4UMRAdlwGWqxvs+jc9i0XFKOEzga8+rONe6WvKyBM4e20I\r\n"
"HcO4BPF92d6sm4qLgyR4oXUkBz6NfDWX8ZdTvXuRaK9qMy1327cCT48sis9F6/eK\r\n"
"QAxx0VHlqGTtxDYjHJPsMLNejuyUDvsaC8TMCSpFTXpMTUvJdmxsF1LZWmIo5lY5\r\n"
"zh0CAwEAAaOBtjCBszAdBgNVHQ4EFgQUBlVzoNJrkkgPkJ8xjuFvVP2E4GswgYMG\r\n"
"A1UdIwR8MHqAFAZVc6DSa5JID5CfMY7hb1T9hOBroVekVTBTMQswCQYDVQQGEwJG\r\n"
"UjEOMAwGA1UECBMFUGFyaXMxDjAMBgNVBAoTBVh5U1NMMSQwIgYDVQQDExtYeVNT\r\n"
"TCBDZXJ0aWZpY2F0ZSBBdXRob3JpdHmCCQDi8PQTFdY8TDAMBgNVHRMEBTADAQH/\r\n"
"MA0GCSqGSIb3DQEBBQUAA4IBAQCGD65b2l5BASFsPvlrcRnLZu/99eWTVAJwJbbD\r\n"
"VhPAQiET0W4U/85EDK7uoFo/SEjyMB/m4T20A8FIDaK7jBPo/1gtbuQjGMRl7h+z\r\n"
"F2iGuNhZ6Td26Uzqclt3oiFtSvDRoZ/9kqkEy7Lrs7FBzOmvfTvrqvADf7cLMa2D\r\n"
"ri/otDpzPr4XoDnwd4C+4bQC/Gr3Uder4VAeTOJtKdGqfYLvPwPSPVBDuVLUybKi\r\n"
"8cMAT6p9IG1e12u6vFqcBT/I67Q0bGU6gzVVz9ZVULXOYZMjjLAfVXC1gesUH2WT\r\n"
"gTEAnEBkSRrkfAi+RezoEFAbmEl3fPt09dwSPku3x7cB3zaJ\r\n"
"-----END CERTIFICATE-----\r\n";

static char dhm_G[] = "4";
static char dhm_P[] = "E4004C1F94182000103D883A448B3F802CE4B44A83301270002C20D0321CFD00" \
"11CCEF784C26A400F43DFB901BCA7538F2C6B176001CF5A0FD16D2C48B1D0C1C" \
"F6AC8E1DA6BCC3B4E1F96B0564965300FFA1D0B601EB2800F489AA512C4B248C" \
"01F76949A60BB7F00A40B1EAB64BDD48E8A700D60B7F1200FA8E77B0A979DABF";


byte_t msg_buf[4096] = { 0 };
dword_t msg_len = 0;

x509_cert* ca_chan = NULL;
x509_cert* ca_srv = NULL;
x509_cert* ca_cli = NULL;
rsa_context* rsa_srv = NULL;
rsa_context* rsa_cli = NULL;
havege_state rng = { 0 };

dhm_context* dhm_srv = NULL;
dhm_context* dhm_cli = NULL;

ecdh_context* ecdh_srv = NULL;
ecdh_context* ecdh_cli = NULL;

byte_t pre_mst[256] = { 0 };
int pre_len = 0;

byte_t mst_srv[48] = { 0 };
byte_t mst_cli[48] = { 0 };
byte_t rnd_srv[32] = { 0 };
byte_t rnd_cli[32] = { 0 };

int key_len = 16;
int mac_len = 20;
int iv_len = 12;

byte_t cli_mac_enc[20] = { 0 };
byte_t cli_mac_dec[20] = { 0 };
byte_t srv_mac_enc[20] = { 0 };
byte_t srv_mac_dec[20];

byte_t cli_iv_enc[16] = { 0 };
byte_t cli_iv_dec[16] = { 0 };
byte_t srv_iv_enc[16] = { 0 };
byte_t srv_iv_dec[16] = { 0 };

byte_t srv_blk[256] = { 0 };
byte_t cli_blk[256] = { 0 };

byte_t srv_mdb[48] = { 0 };
byte_t cli_mdb[48] = { 0 };

arc4_context* cli_arc_enc = NULL;
arc4_context* cli_arc_dec = NULL;
arc4_context* srv_arc_enc = NULL;
arc4_context* srv_arc_dec = NULL;

gcm_context* cli_gcm_enc = NULL;
gcm_context* cli_gcm_dec = NULL;
gcm_context* srv_gcm_enc = NULL;
gcm_context* srv_gcm_dec = NULL;

static void _tls_prf(unsigned char *secret, int slen, char *label, unsigned char *random, int rlen, unsigned char *dstbuf, int dlen)
{
	int nb, hs;
	int i, j, k;
	unsigned char *S1, *S2;
	unsigned char tmp[128] = { 0 };
	unsigned char h_i[20] = { 0 };

	hs = (slen + 1) / 2;
	S1 = secret;
	S2 = secret + slen - hs;

	nb = strlen(label);
	memcpy(tmp + 20, label, nb);
	memcpy(tmp + 20 + nb, random, rlen);
	nb += rlen;

	/*
	* First compute P_md5(secret,label+random)[0..dlen]
	*/
	md5_hmac(S1, hs, tmp + 20, nb, 4 + tmp);

	for (i = 0; i < dlen; i += 16)
	{
		md5_hmac(S1, hs, 4 + tmp, 16 + nb, h_i);
		md5_hmac(S1, hs, 4 + tmp, 16, 4 + tmp);

		k = (i + 16 > dlen) ? dlen % 16 : 16;

		for (j = 0; j < k; j++)
			dstbuf[i + j] = h_i[j];
	}

	/*
	* XOR out with P_sha1(secret,label+random)[0..dlen]
	*/
	sha1_hmac(S2, hs, tmp + 20, nb, tmp);

	for (i = 0; i < dlen; i += 20)
	{
		sha1_hmac(S2, hs, tmp, 20 + nb, h_i);
		sha1_hmac(S2, hs, tmp, 20, tmp);

		k = (i + 20 > dlen) ? dlen % 20 : 20;

		for (j = 0; j < k; j++)
			dstbuf[i + j] = (unsigned char)(dstbuf[i + j] ^ h_i[j]);
	}

	memset(tmp, 0, sizeof(tmp));
	memset(h_i, 0, sizeof(h_i));
}

static void _tls_derive_keys(bool_t cli)
{
	unsigned char rndb[64] = { 0 };
	unsigned char mstb[48] = { 0 };
	unsigned char keyblk[256] = { 0 };

	unsigned char *key1;
	unsigned char *key2;

	xmem_copy((void*)rndb, (void*)rnd_cli, 32);
	xmem_copy((void*)(rndb + 32), (void*)rnd_srv, 32);

	/* generate session master secret*/
	//_tls_prf(rnd_mst, 48, "master secret", rndb, 64, mstb, 48);

	/* Swap the client and server random values.*/
	xmem_copy((void*)rndb, (void*)rnd_srv, 32);
	xmem_copy((void*)(rndb + 32), (void*)rnd_cli, 32);

	//xmem_copy((void*)rnd_cli, (void*)rndb, 32);
	//xmem_copy((void*)rnd_srv, (void*)(rndb + 32), 32);

	/* generate key expansion*/
	_tls_prf(mstb, 48, "key expansion", rndb, 64, keyblk, 256);

	if (cli)
	{
		key1 = keyblk + mac_len * 2;
		key2 = keyblk + mac_len * 2 + key_len;

		xmem_copy(cli_mac_enc, keyblk, mac_len);
		xmem_copy(cli_mac_dec, keyblk + mac_len, mac_len);

		xmem_copy(cli_iv_enc, keyblk + mac_len * 2 + key_len * 2, iv_len);
		xmem_copy(cli_iv_dec, keyblk + mac_len * 2 + key_len * 2 + iv_len, iv_len);

		cli_arc_enc = (arc4_context*)xmem_alloc(sizeof(arc4_context));
		arc4_setup(cli_arc_enc, key1, key_len);
		cli_arc_dec = (arc4_context*)xmem_alloc(sizeof(arc4_context)); 
		arc4_setup(cli_arc_dec, key2, key_len);

		cli_gcm_enc = (gcm_context*)xmem_alloc(sizeof(gcm_context));
		gcm_setkey(cli_gcm_enc, key1, key_len * 8);
		cli_gcm_dec = (gcm_context*)xmem_alloc(sizeof(gcm_context));
		gcm_setkey(cli_gcm_dec, key2, key_len * 8);
	}
	else
	{
		key1 = keyblk + mac_len * 2 + key_len;
		key2 = keyblk + mac_len * 2;

		xmem_copy(srv_mac_dec, keyblk, mac_len);
		xmem_copy(srv_mac_enc, keyblk + mac_len, mac_len);

		xmem_copy(srv_iv_dec, keyblk + mac_len * 2 + key_len * 2, iv_len);
		xmem_copy(srv_iv_enc, keyblk + mac_len * 2 + key_len * 2 + iv_len, iv_len);

		srv_arc_enc = (arc4_context*)xmem_alloc(sizeof(arc4_context)); 
		arc4_setup(srv_arc_enc, key1, key_len);
		srv_arc_dec = (arc4_context*)xmem_alloc(sizeof(arc4_context)); 
		arc4_setup(srv_arc_dec, key2, key_len);

		srv_gcm_enc = (gcm_context*)xmem_alloc(sizeof(gcm_context));
		gcm_setkey(srv_gcm_enc, key1, key_len * 8);
		srv_gcm_dec = (gcm_context*)xmem_alloc(sizeof(gcm_context));
		gcm_setkey(srv_gcm_dec, key2, key_len * 8);
	}
}

void server_key_exchange_dhe()
{
	/*
	struct {
		ServerDHParams params;
		Signature signed_params;
	} ServerKeyExchange;
	*/

	//server write key exchange
	dhm_srv = (dhm_context*)calloc(1, sizeof(dhm_context));
	int n = 0;

	mpi_read_string(&(dhm_srv->P), 16, dhm_P, -1);
	mpi_read_string(&(dhm_srv->G), 16, dhm_G, -1);

	msg_len = 0;
	bool_t rt = dhm_make_params(dhm_srv, 256, msg_buf, &n, havege_rand, &rng);
	msg_len += n;

	unsigned char randb[64];
	xmem_copy((void*)randb, (void*)rnd_cli, 32);
	xmem_copy((void*)(randb + 32), (void*)rnd_srv, 32);

	unsigned char hash[36] = { 0 };

	md5_context md5 = { 0 };
	md5_starts(&md5);
	md5_update(&md5, randb, 64);
	md5_update(&md5, msg_buf, n);
	md5_finish(&md5, hash);
	
	sha1_context sha = { 0 };
	sha1_starts(&sha);
	sha1_update(&sha, randb, 64);
	sha1_update(&sha, msg_buf, n);
	sha1_finish(&sha, hash + MD5_SIZE);

	PUT_SWORD_NET(msg_buf, msg_len, rsa_srv->len);
	msg_len += 2;

	rt = rsa_pkcs1_sign(rsa_srv, RSA_PRIVATE, RSA_RAW, 36, hash, (msg_buf + msg_len));
	msg_len += rsa_srv->len;

	//client read key exchange
	dhm_cli = (dhm_context*)calloc(1, sizeof(dhm_context));

	byte_t* ps = msg_buf;
	byte_t* pe = msg_buf + msg_len;
	
	rt = dhm_read_params(dhm_cli, &ps, pe);

	n = GET_SWORD_NET(ps, 0);
	ps += 2;

	rt = ((int)(pe - ps) == rsa_srv->len) ? 1 : 0;

	n = msg_len - (pe - ps) - 2;

	memset((void*)&md5, 0, sizeof(md5_context));
	md5_starts(&md5);
	md5_update(&md5, randb, 64);
	md5_update(&md5, msg_buf, n);
	md5_finish(&md5, hash);

	memset((void*)&sha, 0, sizeof(sha1_context));
	sha1_starts(&sha);
	sha1_update(&sha, randb, 64);
	sha1_update(&sha, msg_buf, n);
	sha1_finish(&sha, hash + MD5_SIZE);

	rt = rsa_pkcs1_verify(&ca_srv->rsa, RSA_PUBLIC, RSA_RAW, 36, hash, ps);
}

void client_key_exchange_dhe()
{
	/*
	struct {
	ServerDHParams params;
	Signature signed_params;
	} ServerKeyExchange;
	*/

	//client write key exchange
	int n;
	n = dhm_cli->len;

	msg_len = 0;
	PUT_SWORD_NET(msg_buf, msg_len, (unsigned short)n);
	msg_len += 2;

	int rt = dhm_make_public(dhm_cli, 256, msg_buf + msg_len, &n, havege_rand, &rng);
	msg_len += n;

	//if n changed, reset key len
	PUT_SWORD_NET(msg_buf, 0, (unsigned short)n);

	pre_len = dhm_cli->len;

	rt = dhm_calc_secret(dhm_cli, pre_mst, &pre_len);
	
	//server read key exchange
	//Receive G^Y mod P, premaster = (G^Y)^X mod P
	msg_len = 0;
	n = GET_SWORD_NET(msg_buf, msg_len);
	msg_len += 2;

	rt = dhm_read_public(dhm_srv, msg_buf + msg_len, n);
	msg_len += n;

	byte_t premaster[256] = { 0 };
	int prelen = dhm_srv->len;

	rt = dhm_calc_secret(dhm_srv, premaster, &prelen);

	rt = memcmp((void*)pre_mst, (void*)premaster, prelen);
}

void server_key_exchange_ecdh()
{
	/*
	* Ephemeral ECDH parameters:
	*
	* struct {
	*     ECParameters curve_params;
	*     ECPoint      public;
	* } ServerECDHParams;
	*/

	//server write key exchange
	ecdh_srv = (ecdh_context*)calloc(1, sizeof(ecdh_context));
	size_t n = 0;

	ecdh_init(ecdh_srv);

	ecdh_setup(ecdh_srv, ECP_DP_SECP192R1);

	int rt = ecdh_make_params(ecdh_srv, &n, msg_buf, 4096, havege_rand_bytes, &rng);

	msg_len += n;

	unsigned char randb[64];
	xmem_copy((void*)randb, (void*)rnd_cli, 32);
	xmem_copy((void*)(randb + 32), (void*)rnd_srv, 32);

	unsigned char hash[36] = { 0 };

	md5_context md5 = { 0 };
	md5_starts(&md5);
	md5_update(&md5, randb, 64);
	md5_update(&md5, msg_buf, n);
	md5_finish(&md5, hash);

	sha1_context sha = { 0 };
	sha1_starts(&sha);
	sha1_update(&sha, randb, 64);
	sha1_update(&sha, msg_buf, n);
	sha1_finish(&sha, hash + MD5_SIZE);

	PUT_SWORD_NET(msg_buf, msg_len, rsa_srv->len);
	msg_len += 2;

	rt = rsa_pkcs1_sign(rsa_srv, RSA_PRIVATE, RSA_RAW, 36, hash, (msg_buf + msg_len));
	msg_len += rsa_srv->len;

	//client read key exchange
	ecdh_cli = (ecdh_context*)calloc(1, sizeof(ecdh_context));

	ecdh_init(ecdh_cli);

	byte_t* ps = msg_buf;
	byte_t* pe = msg_buf + msg_len;
	
	rt = ecdh_read_params(ecdh_cli, (const unsigned char **)&ps, pe);

	n = GET_SWORD_NET(ps, 0);
	ps += 2;

	rt = ((int)(pe - ps) == rsa_srv->len) ? 1 : 0;

	xmem_copy((void*)randb, (void*)rnd_cli, 32);
	xmem_copy((void*)(randb + 32), (void*)rnd_srv, 32);

	n = msg_len - (pe - ps) - 2;

	memset((void*)&md5, 0, sizeof(md5_context));
	md5_starts(&md5);
	md5_update(&md5, randb, 64);
	md5_update(&md5, msg_buf, n);
	md5_finish(&md5, hash);

	memset((void*)&sha, 0, sizeof(sha1_context));
	sha1_starts(&sha);
	sha1_update(&sha, randb, 64);
	sha1_update(&sha, msg_buf, n);
	sha1_finish(&sha, hash + MD5_SIZE);

	rt = rsa_pkcs1_verify(&ca_srv->rsa, RSA_PUBLIC, RSA_RAW, 36, hash, ps);
}

void client_key_exchange_ecdh()
{
	/*
	struct {
	ServerDHParams params;
	Signature signed_params;
	} ServerKeyExchange;
	*/

	//client write key exchange
	size_t n = 0;

	msg_len = 0;
	PUT_SWORD_NET(msg_buf, msg_len, (unsigned short)n);
	msg_len += 2;

	int rt = ecdh_make_public(ecdh_cli, &n, msg_buf + msg_len, 4096, havege_rand_bytes, &rng);
	msg_len += n;

	//if n changed, reset key len
	PUT_SWORD_NET(msg_buf, 0, (unsigned short)n);

	rt = ecdh_calc_secret(ecdh_cli, &n, pre_mst, 1024, havege_rand_bytes, &rng);
	pre_len = (int)n;

	//server read key exchange
	msg_len = 0;
	n = GET_SWORD_NET(msg_buf, msg_len);
	msg_len += 2;

	rt = ecdh_read_public(ecdh_srv, msg_buf + msg_len, n);
	msg_len += n;

	byte_t premaster[256] = { 0 };

	rt = ecdh_calc_secret(ecdh_srv, &n, premaster, 1024, havege_rand_bytes, &rng);

	rt = memcmp((void*)pre_mst, (void*)premaster, n);
}

void client_finish_ecdh()
{
	byte_t gcm_ctr[8] = { '1', '1', '1', '1', '1', '1', '1', '1' };
	byte_t gcm_add[13];
	byte_t gcm_iv[12];
	byte_t gcm_tag[16];
	byte_t rcv_tag[16];
	int taglen = 16;

	memset((void*)msg_buf, 0x01, 16);
	msg_len = 16;

	xmem_copy((void*)gcm_add, gcm_ctr, SSL_CTR_SIZE);
	gcm_add[SSL_CTR_SIZE] = 22;
	PUT_BYTE(gcm_add, SSL_CTR_SIZE + 1, (byte_t)(0x03));
	PUT_BYTE(gcm_add, SSL_CTR_SIZE + 2, (byte_t)(0x03));
	PUT_SWORD_NET(gcm_add, SSL_CTR_SIZE + 3, (unsigned short)msg_len);

	xmem_copy((void*)gcm_iv, (void*)cli_iv_enc, iv_len);

	gcm_crypt_and_tag(cli_gcm_enc, AES_ENCRYPT, msg_len, gcm_iv, iv_len, gcm_add, 13, msg_buf, msg_buf, taglen, gcm_tag);
	
	xmem_copy((void*)gcm_add, gcm_ctr, SSL_CTR_SIZE);
	gcm_add[SSL_CTR_SIZE] = 22;
	PUT_BYTE(gcm_add, SSL_CTR_SIZE + 1, (byte_t)(0x03));
	PUT_BYTE(gcm_add, SSL_CTR_SIZE + 2, (byte_t)(0x03));
	PUT_SWORD_NET(gcm_add, SSL_CTR_SIZE + 3, (unsigned short)msg_len);

	xmem_copy((void*)gcm_iv, (void*)srv_iv_dec, iv_len);

	gcm_crypt_and_tag(srv_gcm_dec, AES_DECRYPT, msg_len, gcm_iv, iv_len, gcm_add, 13, msg_buf, msg_buf, taglen, rcv_tag);

	int rt = xmem_comp((void*)gcm_tag, 16, rcv_tag, 16);
}

int main(int argc, char* argv[])
{
	xdl_process_init(XDL_APARTMENT_THREAD);

	bool_t rt;

	ca_chan = (x509_cert*)xmem_alloc(sizeof(x509_cert));
	rt = x509parse_crt(ca_chan, (byte_t*)test_ca_crt, a_xslen(test_ca_crt));
	rt = x509parse_crt(ca_chan, (byte_t*)test_cli_crt, a_xslen(test_cli_crt));

	ca_srv = (x509_cert*)xmem_alloc(sizeof(x509_cert));
	rt = x509parse_crt(ca_srv, (byte_t*)test_srv_crt, a_xslen(test_srv_crt));

	ca_cli = (x509_cert*)xmem_alloc(sizeof(x509_cert));
	rt = x509parse_crt(ca_cli, (byte_t*)test_cli_crt, a_xslen(test_cli_crt));

	rsa_srv = (rsa_context*)xmem_alloc(sizeof(rsa_context));
	rt = x509parse_key(rsa_srv, (byte_t*)test_srv_key, a_xslen(test_srv_key), NULL, 0);

	rsa_cli = (rsa_context*)xmem_alloc(sizeof(rsa_context));
	rt = x509parse_key(rsa_cli, (byte_t*)test_cli_key, a_xslen(test_cli_key), NULL, 0);

	int ret;
	rt = x509parse_verify(ca_cli, ca_chan, NULL, &ret);

	havege_init(&rng);

	for (int i = 0; i < 32; i++)
		rnd_srv[i] = (unsigned char)havege_rand(&rng);

	for (int i = 0; i < 32; i++)
		rnd_cli[i] = (unsigned char)havege_rand(&rng);

	for (int i = 0; i < 48; i++)
		mst_cli[i] = (unsigned char)havege_rand(&rng);

	//server_key_exchange_dhe();

	//client_key_exchange_dhe();

	//server_key_exchange_ecdh();

	//client_key_exchange_ecdh();

	_tls_derive_keys(0);

	_tls_derive_keys(1);

	client_finish_ecdh();

	xdl_process_uninit();

	return 0;
}


from setuptools import find_packages, setup

package_name = 'tongsin_py'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='ray',
    maintainer_email='rljw2066@gmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
    'console_scripts': [
        'my_publisher = tongsin_py.my_publisher:main',
        'my_subscriber = tongsin_py.my_subscriber:main',
      ],
    },
)

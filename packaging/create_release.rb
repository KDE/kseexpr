# /usr/bin/env ruby

# This script generates a releasable, signed tarball out of the container repo.
# It also fetches, if its manifest so defines, translations from KDE i18n.
# SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: BSD-3-Clause

# frozen_string_literal: true

require 'fileutils'
require 'json'
require 'optparse'
require 'ostruct'
require 'pathname'

options = OpenStruct.new

opts = OptionParser.new do |o|
  o.on_tail('-h', '--help', 'Show this usage statement') do |_h|
    options.help = true
  end

  o.on_tail('-s', '--sign USER', 'Sign generated tarballs using this key') do |h|
    options.gpg_key = h
  end

  o.on_tail('-k', '--skip', 'Skip base tarball generation') do |_k|
    options.skip_archive = true
  end
end

begin
  opts.parse!(ARGV)
rescue StandardError => e
  warn e, '', opts
  exit 1
end

if options.help
  puts opts
  exit
end

manifest = JSON.parse File.read('manifest.json')

warn 'This script needs a JSON manifest of the underlying repo!' if manifest.empty?

VERSION = (manifest['version'] || `git describe`.chomp[1..-1]).freeze

PREFIX = "#{manifest['name']}-#{VERSION}"

BRANCH = (manifest['gitTag'] || 'HEAD').freeze

puts "Application: #{manifest['name']} #{VERSION}"

ROOT = `git rev-parse --show-toplevel`.chomp.freeze

puts "Repository root: #{ROOT}"

PACKAGING = Pathname(Dir.pwd)

unless options.skip_archive
  if Dir.exist?(PACKAGING / PREFIX)
    puts 'Removing old tarball root...'
    FileUtils.rm_rf PACKAGING / PREFIX
  end

  Dir.chdir(ROOT) do
    puts "Creating tarball root from #{BRANCH}..."
    system "git archive --prefix=#{PREFIX}/ #{BRANCH} | tar xv -C packaging 2> /dev/null", exception: true
  end

  unless manifest['translations'].empty?
    SVN_BASE = 'svn://anonsvn.kde.org/home/kde'

    MAINMODULE = (manifest['translations']['mainmodule'] || 'trunk').freeze

    MODULE = (manifest['translations']['file'] || manifest['name']).freeze

    REVISION = (manifest['translations']['REVISION'] || '').freeze

    puts "Fetching translations (#{MODULE}.po)..."

    Dir.chdir(PACKAGING / PREFIX) do
      languages = `svn cat #{SVN_BASE}/#{MAINMODULE}/l10n-kf5/subdirs`.chomp.lines

      Dir.mkdir('po') unless Dir.exist? 'po'

      Dir.mkdir('l10n') unless Dir.exist? 'l10n'

      languages
        .reject { |lang| lang.start_with? 'x-test' }
        .each do |lang|
          lang.chomp!

          print format('->   %<lang>20s: ', lang: lang)

          pofilename = "#{SVN_BASE}/#{MAINMODULE}/l10n-kf5/#{lang}/messages/kseexpr/#{MODULE}.po"

          system "svn cat #{pofilename} #{REVISION} 2> /dev/null | tee l10n/#{MODULE}.po > /dev/null", exception: true

          if FileTest.size("l10n/#{MODULE}.po").zero?
            puts 'file does not exist.'
            next
          end

          Dir.mkdir "po/#{lang}" unless Dir.exist? "po/#{lang}"

          FileUtils.mv "l10n/#{MODULE}.po", "po/#{lang}/"
          puts 'success!'
        end

      FileUtils.rm_rf 'l10n'
    end
  end
end

Dir.chdir(PACKAGING) do
  puts 'Removing old files...'

  FileUtils.rm Dir.glob("#{PREFIX}.*")

  puts 'Creating tarballs...'

  tar = if system('which gtar > /dev/null 2>&1')
          'gtar'
        else
          'tar'
        end

  system "#{tar} czf #{PREFIX}.tar.gz --group=root --owner=root --sort=name --pax-option=exthdr.name=%d/PaxHeaders/%f,delete=atime,delete=ctime #{PREFIX} > /dev/null", exception: true

  if options.gpg_key
    puts 'Requesting GPG signature...'

    system "gpg --armor --detach-sign -u #{options.gpg_key} #{PREFIX}.tar.gz", exception: true
  end

  puts "Success! Your tarball is at #{File.expand_path("#{PREFIX}.tar.gz")}"
  puts "md5: #{`md5sum #{PREFIX}.tar.gz`.chomp}"
  puts "sha256: #{`sha256sum #{PREFIX}.tar.gz`.chomp}"
end

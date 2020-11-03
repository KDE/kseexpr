#!/usr/bin/env ruby
# This script attempts to make sense of my commits into a decent CHANGELOG.
# SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: BSD-3-Clause

# frozen_string_literal: true

require 'date'
require 'optparse'
require 'ostruct'
require 'pathname'

options = OpenStruct.new

opts = OptionParser.new do |o|
  o.on_tail('-h', '--help', 'Show this usage statement') do |_h|
    options.help = true
  end

  o.on_tail('-f', '--from TAG', 'Start counting from this Git tag...') do |f|
    options.from = f
  end

  o.on_tail('-t', '--to TAG', '...to this tag inclusive') do |t|
    options.to = t
  end
end

begin
  opts.parse!(ARGV)
  system "git rev-parse #{options.from} > /dev/null", exception: true
  system "git rev-parse #{options.to} > /dev/null", exception: true
rescue StandardError => e
  warn e, '', opts
  exit 1
end

if options.help
  puts opts
  exit
end

log = `git log --oneline --reverse #{options.from}..#{options.to}`

date = `git log -1 --format=%aI #{options.to} 2> /dev/null` unless options.to.match? 'HEAD'

log2 = log
       .lines(chomp: true)
       .map { |a| a.split(' ', 2) }
       .map { |a| "#{a[1]} (#{a[0]})" }

log2 = log2.group_by do |a|
  keyword = a.split(' ')[0].downcase
  case keyword
  when 'enable', 'reimplement', 'standardize', 'implement', 'add', 'use'
    'Added'
  when 'automagically', 'rebrand', 'complete', 'automatically', 'hide'
    'Added'
  when 'extract', 'expose', 'improve', 'install', 'factor', 'integrate'
    'Added'
  when 'restore', 'properly', 'sort', 'detect', 'only', 'fix', 'correct'
    'Fixed'
  when 'work', 'do', 'ward', 'propagate', 'ensure', 'forcibly'
    'Fixed'
  when 'differentiate', 'more'
    'Fixed'
  when 'drop', 'assorted', 'cleanup', 'general', 'clean', 'remove'
    'Removed'
  else
    'Changed'
  end
end

log3 = []

# log3 << "# Changelog"
# log3 << "\n"
# log3 << "All notable changes to this project will be documented in this file."
# log3 << "The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),"
# log3 << "and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html)."
# log3 << "\n"

date_string = if date
                "(#{DateTime.iso8601(date).strftime('%B %d, %Y')})"
              else
                ''
              end

version_string = if options.to.match? 'HEAD'
                   'Unreleased'
                 else
                   options.to
                 end

log3 << "## #{version_string} #{date_string}"
log3 << "\n"

%w[Added Fixed Removed Changed].each do |k|
  log3 << "### #{k}"
  log3 << "\n"

  next unless log2[k]

  log2[k].each do |c|
    log3 << "- #{c}"
  end

  log3 << "\n"
end

tarballs = Dir.glob "kseexpr-#{options.to[1..-1]}.tar.gz"

if tarballs
  log3 << '### Release hashes'
  log3 << "\n"

  tarballs.each do |i|
    log3 << "md5sum: #{`md5sum #{i}`}"
    log3 << "sha256: #{`sha256sum #{i}`}"
  end
end

puts log3
